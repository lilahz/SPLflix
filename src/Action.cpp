//
// Created by lilachzi@wincs.cs.bgu.ac.il on 21/11/2019.
//

#include "../include/Action.h"
#include "../include/User.h"

BaseAction::BaseAction() {

}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void CreateUser::act(Session &sess) {

}

std::string CreateUser::toString() const {
    return std::__cxx11::string();
}
