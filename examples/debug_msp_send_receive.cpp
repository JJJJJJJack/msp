#include <Client.hpp>
#include <iostream>
#include <msp_msg.hpp>
#include <unistd.h>   // 用于键盘输入监听（Unix/Linux）
#include <termios.h>  // 用于修改终端模式（无回显、非阻塞）

// 辅助函数：设置终端为非阻塞模式（用于监听按键）
void setNonBlockingTerminal(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable) {
        tty.c_lflag &= ~(ICANON | ECHO);  // 关闭规范模式（无需回车）、关闭回显
        tty.c_cc[VMIN] = 0;               // 最小读取字符数：0（非阻塞）
        tty.c_cc[VTIME] = 0;              // 读取超时：0
    } else {
        tty.c_lflag |= ICANON | ECHO;     // 恢复规范模式和回显
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// 辅助函数：检测是否按下指定按键（示例：Enter键，ASCII码10）
bool isKeyPressed(int keyCode) {
    char ch;
    if (read(STDIN_FILENO, &ch, 1) > 0) {
        return (ch == keyCode);
    }
    return false;
}

int main(int argc, char *argv[]) {
    const std::string device =
        (argc > 1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const size_t baudrate = (argc > 2) ? std::stoul(argv[2]) : 115200;

    msp::client::Client client;
    client.setLoggingLevel(msp::client::LoggingLevel::WARNING);
    client.setVariant(msp::FirmwareVariant::INAV);
    client.start(device, baudrate);

    msp::FirmwareVariant fw_variant = msp::FirmwareVariant::INAV;

    // while(1){
    //     count++;
    //     msp::msg::WayPoint waypoint(fw_variant);
    // 	msp::msg::SetWp set_WP(fw_variant);
    // 	// set_WP.msg1 = count;

    //     // 2. 填充 ServoRotorMixer_t 数据
    //     set_WP.mixer_data.LiftCenter[0] = 100;
    //     set_WP.mixer_data.LiftCenter[1] = 200;
    //     set_WP.mixer_data.LiftCenter[2] = 300;
    //     set_WP.mixer_data.RotationAxis[0] = 1000;
    //     set_WP.mixer_data.RotationAxis[1] = 0;
    //     set_WP.mixer_data.RotationAxis[2] = 0;
    //     set_WP.mixer_data.t0[0] = 0;
    //     set_WP.mixer_data.t0[1] = 1000;
    //     set_WP.mixer_data.t0[2] = 0;
    //     set_WP.mixer_data.T_eq = 5000;
    //     set_WP.mixer_data.theta_eq = 150;
    //     set_WP.mixer_data.prop_rot_dir = 1; // CCW
    //     set_WP.mixer_data.tau_scaler[0] = 100;
    //     set_WP.mixer_data.tau_scaler[1] = 200;
    //     set_WP.mixer_data.tau_scaler[2] = 300;

    //     // 3. 编码并发送（假设已有 MSP 发送接口）
    //     // auto data = set_WP.encode();
    //     // if (data) {
    //     //     // send_msp_data(data->data(), data->size(), msp::ID::MSP_SET_WP); // 替换为实际发送函数
    //     // }
    //     if(client.sendMessage(set_WP) == 1) {
    //         std::cout<<"sending wp"<<std::endl;
    //     }

    //     if(client.sendMessage(waypoint) == 1) {
    //         printf("msg1: %6.2f,     msg2: %6.2f,     msg3: %6.2f,     msg4: %6.2f,     msg5: %6.2f,     msg6: %6.2f\n", (double)waypoint.msg1, (double)waypoint.msg2, (double)waypoint.msg3, (double)waypoint.msg4, (double)waypoint.msg5, (double)waypoint.msg6);
    //     }

    //     // msp::msg::SetRawRc RawRC(fw_variant);
    //     // RawRC.channels.push_back(1500);
    //     // RawRC.channels.push_back(1500);
    //     // RawRC.channels.push_back(885);
    //     // RawRC.channels.push_back(1500);
    //     // RawRC.channels.push_back(1500);
    //     // RawRC.channels.push_back(1500);
    //     // RawRC.channels.push_back(1500);
    //     // if(count > 200)
    //     //     RawRC.channels.push_back(1900);
    //     // else
    //     //     RawRC.channels.push_back(1000);
    //     // if(client.sendMessage(RawRC) == 1) {
    //     //     std::cout<<"sending RawRC"<<std::endl;
    //     // }

    //     msp::msg::Attitude attitude(fw_variant);
    //     if(client.sendMessage(attitude) == 1)
    //         std::cout << attitude;
    //     usleep(20000);
    // }

    // 4. 监听按键，按下Enter后发送一次指令
    bool sent = false;  // 标记是否已发送
    while (1) {
        // 发送WayPoint指令（原逻辑）
        msp::msg::WayPoint waypoint(fw_variant);
        if (client.sendMessage(waypoint) == 1) {
            printf("WayPoint发送成功 | msg1: %6.2f, msg2: %6.2f, msg3: %6.2f, msg4: %6.2f, msg5: %6.2f, msg6: %6.2f\n",
                    (double)waypoint.msg1, (double)waypoint.msg2, (double)waypoint.msg3,
                    (double)waypoint.msg4, (double)waypoint.msg5, (double)waypoint.msg6);
        } else {
            std::cerr << "WayPoint发送失败！" << std::endl;
        }
        usleep(10000);  // 轻微延时，降低CPU占用
        // 检测是否按下Enter键（ASCII码10）
        if (!sent && isKeyPressed(10)) {
            std::cout << "\n检测到Enter键，开始发送指令..." << std::endl;
            
            // ========== 核心发送逻辑（仅执行一次）==========
            msp::FirmwareVariant fw_variant = msp::FirmwareVariant::INAV;
            int count = 1;  // 仅发送一次，count固定为1
            

            // （可选）如需发送SetWp/Attitude/RawRC，取消以下注释
            msp::msg::SetWp set_WP(fw_variant);
            set_WP.mixer_data.LiftCenter[0] = 100;
            set_WP.mixer_data.LiftCenter[1] = 200;
            set_WP.mixer_data.LiftCenter[2] = 300;
            set_WP.mixer_data.RotationAxis[0] = 1000;
            set_WP.mixer_data.RotationAxis[1] = 0;
            set_WP.mixer_data.RotationAxis[2] = 0;
            set_WP.mixer_data.t0[0] = 0;
            set_WP.mixer_data.t0[1] = 1000;
            set_WP.mixer_data.t0[2] = 0;
            set_WP.mixer_data.T_eq = 5000;
            set_WP.mixer_data.theta_eq = 150;
            set_WP.mixer_data.prop_rot_dir = 1; // CCW
            set_WP.mixer_data.tau_scaler[0] = 100;
            set_WP.mixer_data.tau_scaler[1] = 200;
            set_WP.mixer_data.tau_scaler[2] = 300;
            if (client.sendMessage(set_WP) == 1) {
                std::cout << "SetWp发送成功！" << std::endl;
            }

            // msp::msg::Attitude attitude(fw_variant);
            // if (client.sendMessage(attitude) == 1) {
            //     std::cout << "Attitude发送成功：" << attitude << std::endl;
            // }

            sent = true;  // 标记为已发送，退出循环
            std::cout << "指令发送完成！ 发送了"<< count << "次" << std::endl;
            usleep(10000);  // 轻微延时，降低CPU占用
        }

    }

    // 5. 恢复终端模式 + 关闭客户端
    setNonBlockingTerminal(false);
    client.stop();

    return 0;
}
    
//     client.stop();
// }
