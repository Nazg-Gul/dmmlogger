// DMMLogger - A logging software for digital multimeters
// Copyright 2014 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#include <cassert>

#include "dmm/dmm_factory.h"

#ifdef WITH_DUMMY_DMM
#  include "dmm/dummy_dmm.h"
#endif
#include "dmm/ut60e.h"

DMMFactory::DMMFactory(void) {
#ifdef WITH_DUMMY_DMM
  RegisterModel(DMM_MODEL_TYPE_DUMMY, "Dummy");
#endif
  RegisterModel(DMM_MODEL_TYPE_UT60E, "UT60E");
}

void DMMFactory::RegisterModel(DMMModelType type, std::string name) {
  DMMModel model;
  model.type = type;
  model.name = name;
  models_.push_back(model);
}

DMM *DMMFactory::CreateDMM(DMMModelType type, const std::string &device) {
  DMM *dmm = NULL;
  switch (type) {
#ifdef WITH_DUMMY_DMM
    case DMM_MODEL_TYPE_DUMMY:
      dmm = new DummyDMM();
      break;
#endif
    case DMM_MODEL_TYPE_UT60E:
      dmm = new UT60E(device);
      break;
    default:
      assert(!"Unknown model type passed to the factory.");
  }
  return dmm;
}
