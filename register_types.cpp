#include "register_types.h"
#include "anl_noise.h"
#include "noise.h"

void register_anl_types() {

	ClassDB::register_class<AnlNoise>();
	ClassDB::register_class<AccidentalFractalNoise>();
}

void unregister_anl_types() {

	// nothing to do here
}
