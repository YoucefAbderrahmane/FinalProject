/*
 * test.cpp
 *
 *  Created on: 2 août 2016
 *      Author: root
 */

#include <node/node.h>
//#include <node/v8.h>


#include "../Model/Observation.h"

namespace demo {

//using v8::Exception;
//using v8::Number;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
//using v8::Null;
using v8::Object;
using v8::String;
using v8::Value;


void CreateObject(const FunctionCallbackInfo<Value>& args) {
  MyObject::NewInstance(args);
}


void InitAll(Local<Object> exports, Local<Object> module) {
  MyObject::Init(exports->GetIsolate());

  NODE_SET_METHOD(module, "exports", CreateObject);
}

NODE_MODULE(test, InitAll)

}
