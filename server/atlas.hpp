#ifndef ATLAS_HPP
#define ATLAS_HPP

#include <vector>
#include <easyx.h>

class Atlas {
public:
	Atlas() = default;
	~Atlas() = default;

	void load(LPCTSTR path_template, int num) {
		_img_list.clear();
		_img_list.resize(num);

		TCHAR path_file[256];
		for (auto i = 0; i < num; i++) {
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&_img_list[i], path_file);
		}
	}

	void clear() {
		_img_list.clear();
	}

	int get_size() const {
		return _img_list.size();
	}

	IMAGE* get_image(int idx) {
		return &_img_list[idx];
	}

	void add_image(const IMAGE& img) {
		_img_list.emplace_back(img);
	}

private:
	std::vector<IMAGE> _img_list;
};

#endif // !ATLAS_HPP
