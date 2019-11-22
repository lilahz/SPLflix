//
// Created by lilachzi@wincs.cs.bgu.ac.il on 21/11/2019.
//

#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"

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



CreateUser::CreateUser(const std::string &name, const std::string &algo) : newUserName(name),algo(algo) {}

void CreateUser::act(Session &sess) {
    User* user;
    if (algo == "len") {
        user = new LengthRecommenderUser(newUserName);
    }
    else if (algo == "rer") {
        user = new RerunRecommenderUser(newUserName);
    }
    if (algo == "gen") {
        user = new GenreRecommenderUser(newUserName);
    }
    sess.addToUserMap(newUserName , user);
    user->setNotSeen(sess.getContent());
    //TODO adding the content list all the watchables.
}

std::string CreateUser::toString() const {
    return std::__cxx11::string();
}


ChangeActiveUser::ChangeActiveUser(User *user) : activeUser(user) {}

void ChangeActiveUser::act(Session &sess) {
     sess.setActiveUser(activeUser);
}

std::string ChangeActiveUser::toString() const {
    return activeUser->getName();
}

DeleteUser::DeleteUser(User *user) : userToDelete(user) {}

void DeleteUser::act(Session &sess) {
    sess.deleteFromUserMap(userToDelete->getName());
}

std::string DeleteUser::toString() const {
    return std::__cxx11::string();
}

DuplicateUser::DuplicateUser(User *user, std::string newName):userToDuplicate(user), newName(newName){}

void DuplicateUser::act(Session &sess) {
    BaseAction* newUser =new CreateUser(newName, userToDuplicate->getAlgo());
    newUser->act(sess);
}

std::string DuplicateUser::toString() const {
    return std::__cxx11::string();
}

void PrintContentList::act(Session &sess) {

}

std::string PrintContentList::toString() const {
    return std::__cxx11::string();
}

void PrintWatchHistory::act(Session &sess) {

}

std::string PrintWatchHistory::toString() const {
    return std::__cxx11::string();
}

void Watch::act(Session &sess) {

}

std::string Watch::toString() const {
    return std::__cxx11::string();
}

void PrintActionsLog::act(Session &sess) {

}

std::string PrintActionsLog::toString() const {
    return std::__cxx11::string();
}

void Exit::act(Session &sess) {

}

std::string Exit::toString() const {
    return std::__cxx11::string();
}
