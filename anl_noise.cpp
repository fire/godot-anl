/*************************************************************************/
/*  noise.cpp                                                            */
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

#include "anl_noise.h"
#include "noise.h"

#include "core/core_string_names.h"

void AccidentalNoise::_generate_vbm(int p_size) {

	Index period_index = anl_noise->constant(real_t(p_size) / real_t(get_period()) / real_t(4.0));
	Index fractal = anl_noise->fractal(seed_index, fractal_layer, persistence_index, lacunarity_index, octave_index, period_index);
	layer = anl_noise->clamp(fractal, anl_noise->zero(), anl_noise->one()); //discard lower range noise
}

AccidentalNoise::AccidentalNoise() {

	anl_noise.instance();

	seed = 0;
	period = 64.0f;
	persistence = 0.5f;
	octaves = 3;
	lacunarity = 2.0f;
	_init_seeds();
}

AccidentalNoise::~AccidentalNoise() {
}

void AccidentalNoise::_init_seeds() {
	Index quintic = anl_noise->constant(3);
	//Notice quintic interpolation is passed by index, not enum value.
	//I think this is done to use different kind of interpolation at each coordinate... or a bug
	seed_index = anl_noise->constant(get_seed());
	fractal_layer = anl_noise->fractal_layer(anl::BasisTypes::BASIS_SIMPLEX, quintic, 1, 10, 1, 1);
	persistence_index = anl_noise->constant(get_persistence());
	lacunarity_index = anl_noise->constant(-get_lacunarity());
	octave_index = anl_noise->constant(get_octaves());
}

void AccidentalNoise::set_seed(int p_seed) {
	if (seed == p_seed)
		return;

	seed = p_seed;
	_init_seeds();
	emit_changed();
}

int AccidentalNoise::get_seed() const {

	return seed;
}

void AccidentalNoise::set_octaves(int p_octaves) {
	if (p_octaves == octaves) return;
	octaves = CLAMP(p_octaves, 1, 6);
	_init_seeds();
	emit_changed();
}

int AccidentalNoise::get_octaves() const {
	return octaves;
}

void AccidentalNoise::set_period(float p_period) {
	ERR_FAIL_COND(p_period <= 0);
	period = p_period;
	_init_seeds();
	emit_changed();
}

float AccidentalNoise::get_period() const
{
	return period;
}

void AccidentalNoise::set_persistence(float p_persistence) {
	if (p_persistence == persistence) return;
	persistence = p_persistence;
	_init_seeds();
	emit_changed();
}

float AccidentalNoise::get_persistence() const {
	return persistence;
}

void AccidentalNoise::set_lacunarity(float p_lacunarity) {

	if (p_lacunarity == lacunarity) return;
	lacunarity = p_lacunarity;
	_init_seeds();
	emit_changed();
}

float AccidentalNoise::get_lacunarity() const {
	return lacunarity;
}

Ref<Image> AccidentalNoise::get_image(int p_width, int p_height) {
	return anl_noise->map_to_image(Vector2(p_width, p_height), layer, anl::SEAMLESS_NONE);
}

Vector<Ref<Image> > AccidentalNoise::get_image_3d(int p_x, int p_y, int p_z) {
	int max = MAX(p_x, MAX(p_y, p_z));
	_generate_vbm(max);
	return anl_noise->map_to_image_3d(Vector3(p_x, p_y, p_z), layer, anl::SEAMLESS_NONE, AABB(Vector3(0, 0, 0), Vector3(1, 1, 1)));
}

Ref<Image> AccidentalNoise::get_seamless_image(int p_size) {
	_generate_vbm(p_size);
	return anl_noise->map_to_image(Vector2(p_size, p_size), layer, anl::SEAMLESS_XY);
}

Vector<Ref<Image> > AccidentalNoise::get_seamless_image_3d(int p_size) {
	ERR_FAIL_COND_V(anl_noise.ptr() == NULL, Vector<Ref<Image> >())
	_generate_vbm(p_size);
	return anl_noise->map_to_image_3d(Vector3(p_size, p_size, p_size), layer, anl::SEAMLESS_XYZ, AABB(Vector3(0, 0, 0), Vector3(1, 1, 1)));
}

void AccidentalNoise::_bind_methods() {
}

float AccidentalNoise::get_noise_2d(float x, float y) {
	_generate_vbm(MAX(x, y));
	return anl_noise->get_scalar_2d(x, y, layer);
}

float AccidentalNoise::get_noise_3d(float x, float y, float z) {
	int max = MAX(x, MAX(y, z));
	_generate_vbm(max);
	return anl_noise->get_scalar_3d(x, y, z, layer);
}

float AccidentalNoise::get_noise_4d(float x, float y, float z, float w) {
	int max = MAX(x, MAX(y, MAX(z, w)));
	_generate_vbm(max);
	return anl_noise->get_scalar_4d(x, y, z, w, layer);
}
