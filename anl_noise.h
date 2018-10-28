/*************************************************************************/
/*  open_noise.h                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef ACCIDENTAL_FRACTAL_NOISE_H
#define ACCIDENTAL_FRACTAL_NOISE_H

#include "core/image.h"
#include "core/reference.h"
#include "noise.h"
#include "scene/resources/noise.h"
#include "scene/resources/texture.h"

class AccidentalFractalNoise  : public Noise {
	GDCLASS(AccidentalFractalNoise , Noise)
	OBJ_SAVE_TYPE(AccidentalFractalNoise );

	Ref<AnlNoise> anl_noise;

	int32_t seed;
	float persistence; // Controls details, value in [0,1]. Higher increases grain, lower increases smoothness.
	int32_t octaves; // Number of noise layers

	Index layer;
	Index fractal_layer;
	Index seed_index;
	Index persistence_index;
	Index lacunarity_index;
	Index octave_index;

	float period; // Distance above which we start to see similarities. The higher, the longer "hills" will be on a terrain.
	float lacunarity; // Controls period change across octaves. 2 is usually a good value to address all detail levels.

	void _generate_vbm(int p_size);

public:
	AccidentalFractalNoise ();
	~AccidentalFractalNoise ();

	void _init_seeds();

	virtual void set_seed(int seed);
	virtual int get_seed() const;

	virtual void set_period(float p_period);
	virtual float get_period() const;

	virtual void set_octaves(int p_octaves);
	virtual int get_octaves() const;

	virtual void set_persistence(float p_persistence);
	virtual float get_persistence() const;

	virtual void set_lacunarity(float p_lacunarity);
	virtual float get_lacunarity() const;

	virtual Ref<Image> get_image(int p_width, int p_height);
	virtual Vector<Ref<Image> > get_image_3d(int p_x, int p_y, int p_layer);
	virtual Ref<Image> get_seamless_image(int p_size);
	virtual Vector<Ref<Image> > get_seamless_image_3d(int p_size);
	virtual float get_noise_2d(float x, float y);
	virtual float get_noise_3d(float x, float y, float z);
	virtual float get_noise_4d(float x, float y, float z, float w);

protected:
	static void _bind_methods();
};

#endif // ACCIDENTAL_FRACTAL_NOISE_H
