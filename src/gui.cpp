#include "client.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

class VerticalBar : public Fl_Widget {
    int _value;

    public:
        VerticalBar(int X, int Y, int W, int H) : Fl_Widget(X,Y,W,H), _value(0) {}

        void value(int v) { 
            if (_value != v ) {
                _value = v;
                redraw();
            } 
        }

        void draw() override {
            fl_color(FL_DARK3);
            fl_rectf(x(), y(), w(), h());

            if (_value > 70) fl_color(FL_RED);
            else if (_value > 50) fl_color(FL_DARK_YELLOW);
            else if (_value > 30) fl_color(FL_YELLOW);
            else fl_color(FL_GREEN);

            int bar_h = h() * _value / 100;
            fl_rectf(x(), y() + h() - bar_h, w(), bar_h);
        }
};

VerticalBar* cpu_bar;
Fl_Box* cpu_label;

VerticalBar* mem_bar;
Fl_Box* mem_label;

std::atomic<bool> stop_flag(false);
Fl_Window* win;

Client client;
Client::sys_metrics data;

void handle_signal(int) { 
    stop_flag = true;
    if(win) win->hide();
}

void ui_update(void* userdata) {
    Client::sys_metrics* data = static_cast<Client::sys_metrics*>(userdata);

    cpu_bar->value(data->cpu);
    mem_bar->value(data->mem);

    cpu_label->copy_label(("CPU : " + std::to_string(data->cpu) + "%").c_str());
    mem_label->copy_label(("MEM : " + std::to_string(data->mem) + "%").c_str());

    delete data;
}

void update_metrics() {
    while(!stop_flag) {
        client.call();
        client.get_data();

        Client::sys_metrics* metrics = new Client::sys_metrics(client.get_metrics());

        Fl::awake(ui_update, metrics);

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    std::signal(SIGINT, handle_signal);
    
    int bar_width = 30;
    int bar_height = 100;

    int label_height = 22;
    int label_width = 70;
    
    int spacing_vertical = 15;
    
    int top_padding = 5;
    int side_padding = 5;

    int win_width = bar_width + 2 * side_padding;
    int win_height = top_padding + label_height + bar_height
                   + spacing_vertical + label_height + bar_height + top_padding;

    win = new Fl_Window(win_width, win_height, "system-monitor");
    win->border(0);
    win->color(FL_BLACK);

    int start_x = side_padding;

    /* CPU */
    cpu_label = new Fl_Box(start_x, top_padding, label_width, label_height, "CPU : ...");
    cpu_label->labelcolor(FL_WHITE);
    cpu_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    cpu_label->labelsize(12);

    int cpu_bar_y = top_padding + label_height;
    cpu_bar = new VerticalBar(start_x + (label_width - bar_width)/2, cpu_bar_y, bar_width, bar_height);

    /* MEM */
    int mem_label_y = cpu_bar_y + bar_height + spacing_vertical;
    mem_label = new Fl_Box(start_x, mem_label_y, label_width, label_height, "MEM : ...");
    mem_label->labelcolor(FL_WHITE);
    mem_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    mem_label->labelsize(12);

    int mem_bar_y = mem_label_y + label_height;
    mem_bar = new VerticalBar(start_x + (label_width - bar_width)/2, mem_bar_y, bar_width, bar_height);

    Fl::lock();
    std::thread t(update_metrics);

    win->end();
    win->show();

    int result = Fl::run();

    t.join();

    return result;
}