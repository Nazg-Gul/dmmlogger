// DMMLogger - A logging software for digital multimeters
// Copyright 2014 Sergey Sharybin. All rights reserved.
//
// Author: sergey.vfx@gmail.com (Sergey Sharybin)

#ifndef DMM_FACTORY_H_
#define DMM_FACTORY_H_

#include <string>
#include <vector>

#include "dmm/dmm.h"

class DMMFactory {
 public:
  enum DMMModelType {
#ifdef WITH_DUMMY_DMM
    DMM_MODEL_TYPE_DUMMY,
#endif
    DMM_MODEL_TYPE_UT60E,
  };

  struct DMMModel {
    DMMModelType type;
    std::string name;
  };

  DMMFactory(void);

  const std::vector<DMMModel> &GetModels(void) {
    return models_;
  }

  DMM *CreateDMM(DMMModelType type, const std::string &device);

 private:
  void RegisterModel(DMMModelType type, std::string name);

  std::vector<DMMModel> models_;
};

#endif  // DMM_FACTORY_H_
