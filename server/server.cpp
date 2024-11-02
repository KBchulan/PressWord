#include "../httplib.hpp"

#include <mutex>
#include <thread>
#include <fstream>

std::mutex g_mutex;		// 全局互斥锁
std::string str_text;	// 文本内容

int process_1 = -1;		// 玩家1进度
int process_2 = -1;		// 玩家2进度

int main(int argc, char** argv) {
	std::ifstream file("text.txt");

	if (!file.good()) {
		MessageBox(nullptr, L"无法打开文本文件", L"启动失败", MB_OK | MB_ICONERROR);
		return -1;
	}

	std::stringstream str_stream;
	str_stream << file.rdbuf();
	str_text = str_stream.str();

	file.close();

	httplib::Server server;

	server.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
		std::lock_guard<std::mutex> locker(g_mutex);

		if (process_1 >= 0 && process_2 >= 0) {
			res.set_content("-1", "text/plain");
			return -1;
		}

		res.set_content(process_1 >= 0 ? "2" : "1", "text/plain");
		(process_1 >= 0) ? (process_2 >= 0) : (process_1 = 0);
		});

	server.Post("/query_text", [&](const httplib::Request& req, httplib::Response& res) {
		res.set_content(str_text, "text/plain");
		});

	server.Post("/update_1", [&](const httplib::Request& req, httplib::Response& res) {
		std::lock_guard<std::mutex> locker(g_mutex);

		process_1 = std::stoi(req.body);

		res.set_content(std::to_string(process_2), "text/plain");
		});
	
	server.Post("/update_2", [&](const httplib::Request& req, httplib::Response& res) {
		std::lock_guard<std::mutex> locker(g_mutex);

		process_2 = std::stoi(req.body);

		res.set_content(std::to_string(process_1), "text/plain");
		});

	server.listen("0.0.0.0", 25565);

	return 0;
}