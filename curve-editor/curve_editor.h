#pragma once
#include <memory>
#include "../curve-lib/curve.h"
#include "../editor-lib/transformation.h"
#include "edit_control.h"

class curve_editor
{
public:
	document_model& document;
	size_t curve_index;

	struct rprops
	{
		transformation curve_to_screen;
		transformation screen_to_curve;
	};

	struct nearest_result
	{
		float distance;
		std::unique_ptr<edit_control> control;
		void reset();
	};

	bool is_selection_valid() {
		return curve_index >= 0 && curve_index < document.curve_list.size();
	}

	nearest_result get_nearest_edit_control(float x, float y, const transformation& curve_to_screen);
	void remove_zero_length_segments();
	void change_param_count(int delta, float x);
};
