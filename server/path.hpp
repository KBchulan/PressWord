#ifndef PATH_HPP
#define PATH_HPP

#include "vector2.hpp"

#include <vector>

class Path {
public:
	explicit Path(const std::vector<Vector2>& point_list) {
		this->point_list = point_list;

		for (auto i = 1; i < point_list.size(); i++) {
			float segment_len = (point_list[i] - point_list[i - 1]).length();
			segment_len_list.emplace_back(segment_len);
			total_length += segment_len;
		}
	}
	~Path() = default;
	
	Vector2 get_position_at_progess(float progess) const {
		if (progess <= 0) return point_list.front();
		if (progess >= 1) return point_list.back();

		float target_distance = total_length * progess;
		float accumulated_len = 0.0f;

		for (auto i = 1; i < point_list.size(); i++) {
			accumulated_len += segment_len_list[i - 1];
			if (accumulated_len >= target_distance) {
				float segment_process = (target_distance - (accumulated_len - segment_len_list[i - 1])) / segment_len_list[i - 1];
				return point_list[i - 1] + (point_list[i] - point_list[i - 1]) * segment_process;
			}
		}
		return point_list.back();
	}

private:
	float total_length = 0;
	std::vector<Vector2> point_list;
	std::vector<float> segment_len_list;

};

#endif // !PATH_HPP
