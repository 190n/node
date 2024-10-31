#include <assert.h>
#include <js_native_api.h>
#include <string.h>
#include "../common.h"
#include "../entry_point.h"

static napi_value HasElement(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value target = args[0];
  napi_value element = args[1];
  napi_status status;

  uint32_t index = -1;

  NODE_API_CALL(env, napi_get_value_uint32(env, element, &index));
  
  bool success = false;
  NODE_API_CALL(env, napi_has_element(env, target, index, &success));

  napi_value out;

  NODE_API_CALL(env, napi_get_boolean(env, success, &out));

  return out;
}

static napi_value TestExceptions(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value args[1];
  NODE_API_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value target = args[0];
  napi_value exception, key, value;
  napi_status status;
  bool is_exception_pending;
  bool bool_result;

  NODE_API_CALL(env,
                napi_create_string_utf8(env, "key", NAPI_AUTO_LENGTH, &key));
  NODE_API_CALL(
      env, napi_create_string_utf8(env, "value", NAPI_AUTO_LENGTH, &value));

#define PROCEDURE(call)                                                        \
  {                                                                            \
    status = (call);                                                           \
    NODE_API_ASSERT(                                                           \
        env, status == napi_pending_exception, "expect exception pending");    \
    NODE_API_CALL(env, napi_is_exception_pending(env, &is_exception_pending)); \
    NODE_API_ASSERT(env, is_exception_pending, "expect exception pending");    \
    NODE_API_CALL(env, napi_get_and_clear_last_exception(env, &exception));    \
  }
  // discard the exception values.

  // properties
  printf("napi_set_property\n");
  PROCEDURE(napi_set_property(env, target, key, value));
  printf("napi_set_named_property\n");
  PROCEDURE(napi_set_named_property(env, target, "key", value));
  printf("napi_has_property\n");
  PROCEDURE(napi_has_property(env, target, key, &bool_result));
  printf("napi_has_own_property\n");
  PROCEDURE(napi_has_own_property(env, target, key, &bool_result));
  printf("napi_has_named_property\n");
  PROCEDURE(napi_has_named_property(env, target, "key", &bool_result));
  printf("napi_get_property\n");
  PROCEDURE(napi_get_property(env, target, key, &value));
  printf("napi_get_named_property\n");
  PROCEDURE(napi_get_named_property(env, target, "key", &value));
  printf("napi_delete_property\n");
  PROCEDURE(napi_delete_property(env, target, key, &bool_result));

  // elements
  printf("napi_set_element\n");
  PROCEDURE(napi_set_element(env, target, 0, value));
  printf("napi_has_element\n");
  PROCEDURE(napi_has_element(env, target, 0, &bool_result));
  printf("napi_get_element\n");
  PROCEDURE(napi_get_element(env, target, 0, &value));
  printf("napi_delete_element\n");
  PROCEDURE(napi_delete_element(env, target, 0, &bool_result));

  napi_property_descriptor descriptors[] = {
      DECLARE_NODE_API_PROPERTY_VALUE("key", value),
  };
  printf("napi_define_properties\n");
  PROCEDURE(napi_define_properties(
      env, target, sizeof(descriptors) / sizeof(*descriptors), descriptors));

  printf("napi_get_all_property_names\n");
  PROCEDURE(napi_get_all_property_names(env,
                                        target,
                                        napi_key_own_only,
                                        napi_key_enumerable,
                                        napi_key_keep_numbers,
                                        &value));
  printf("napi_get_property_names\n");
  PROCEDURE(napi_get_property_names(env, target, &value));

  return NULL;
}

EXTERN_C_START
napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor descriptors[] = {
      DECLARE_NODE_API_PROPERTY("testExceptions", TestExceptions),
      DECLARE_NODE_API_PROPERTY("hasElement", HasElement),
  };

  NODE_API_CALL(
      env,
      napi_define_properties(env,
                             exports,
                             sizeof(descriptors) / sizeof(*descriptors),
                             descriptors));

  return exports;
}
EXTERN_C_END
