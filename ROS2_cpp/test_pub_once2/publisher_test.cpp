#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses a fancy C++11 lambda
 * function to shorten the callback syntax, at the expense of making the
 * code somewhat more difficult to understand at first glance. */

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher(int number)
  : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    auto timer_callback =
      [this,number]() -> void {
        std::cout << number << "\n";
        auto message = std_msgs::msg::String();
        message.data = "Hello, world! " + std::to_string(this->count_++);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
        this->publisher_->publish(message);
      };

    timer_ = this->create_wall_timer(0ms, timer_callback);
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};


int main(int argc, char * argv[])
{
  rclcpp::WallRate rate(1s);
  rclcpp::init(argc, argv);
  //rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::executors::MultiThreadedExecutor exec;
  auto pub_node = std::make_shared<MinimalPublisher>(10);
  exec.add_node(pub_node);
  exec.spin();
  for(int i=0; i < 100; i++){
    std::cout << "hoge\n";
  }

  rclcpp::shutdown();
  return 0;
}