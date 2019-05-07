#include <libndls.h>

#include "mpconfig.h"
#include "misc.h"
#include "nlr.h"
#include "qstr.h"
#include "obj.h"
#include "runtime.h"
#include "objtuple.h"
#include "texture.h"

static mp_obj_t nsp_readRTC()
{
	return mp_obj_new_int(*(unsigned int*)0x90090000);
}
static MP_DEFINE_CONST_FUN_OBJ_0(nsp_readRTC_obj, nsp_readRTC);

static mp_obj_t nsp_waitKeypress()
{
	wait_key_pressed();
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(nsp_waitKeypress_obj, nsp_waitKeypress);

static mp_obj_t nsp_peekMemory(mp_obj_t address) {
	return mp_obj_new_int(*(unsigned volatile int*)(mp_obj_get_uint(address)));
}
static MP_DEFINE_CONST_FUN_OBJ_1(nsp_peekMemory_obj, nsp_peekMemory);

static mp_obj_t nsp_pokeMemory(mp_obj_t address, mp_obj_t value) {
	*(unsigned volatile int*)(mp_obj_get_int(address)) = mp_obj_get_uint(value);
	return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(nsp_pokeMemory_obj, nsp_pokeMemory);

STATIC const mp_map_elem_t mp_module_nsp_globals_table[] = {
	{ MP_OBJ_NEW_QSTR(MP_QSTR_Texture), (mp_obj_t) &nsp_texture_type },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_waitKeypress), (mp_obj_t) &nsp_waitKeypress_obj },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_readRTC), (mp_obj_t) &nsp_readRTC_obj },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_peekMemory), (mp_obj_t) &nsp_peekMemory_obj },
	{ MP_OBJ_NEW_QSTR(MP_QSTR_pokeMemory), (mp_obj_t) &nsp_pokeMemory_obj }
};

STATIC const mp_obj_dict_t mp_module_nsp_globals = {
    .base = {&mp_type_dict},
    .map = {
        .all_keys_are_qstrs = 1,
        .table_is_fixed_array = 1,
        .used = MP_ARRAY_SIZE(mp_module_nsp_globals_table),
        .alloc = MP_ARRAY_SIZE(mp_module_nsp_globals_table),
        .table = (mp_map_elem_t*) mp_module_nsp_globals_table,
    },
};

const mp_obj_module_t mp_module_nsp = {
    .base = { &mp_type_module },
    .name = MP_QSTR_nsp,
    .globals = (mp_obj_dict_t*) &mp_module_nsp_globals,
};
