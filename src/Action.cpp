//
// Created by lilachzi@wincs.cs.bgu.ac.il on 21/11/2019.
//

#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

BaseAction::BaseAction() {
    status= PENDING;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;

}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}



//=================================================Create User==========================================================

CreateUser::CreateUser(const std::string &name, const std::string &algo) : newUserName(name),algo(algo) {}

void CreateUser::act(Session &sess) {
    User* user;
    std::string name, thirdString;
    std::cin >> name;
    std::cin >> thirdString;
    if ((thirdString != "len") & (thirdString != "rer") & (thirdString != "gen")) {
        //                actionsLog.push_back()
        error("Invalid algorithm input.");
        std::cout<< "error" << getErrorMsg();
    }
    else if (sess.findInUserMap(name) != nullptr) {
       error("User already exist.");
        std::cout<< "error" << getErrorMsg();
    }
    else if (algo == "len") {
        user = new LengthRecommenderUser(newUserName);
        complete();
    }
    else if (algo == "rer") {
        user = new RerunRecommenderUser(newUserName);
        complete();
    }
    else if (algo == "gen") {
        user = new GenreRecommenderUser(newUserName);
        complete();
    }
    sess.addToUserMap(newUserName , user);
    sess.addToActionsLog(this);


}

std::string CreateUser::toString() const {
    if (getStatus() == ERROR){
        return "ERROR: - " + getErrorMsg();
    }
    return "CreateUser " +  getStatus() ;
}

//==============================================Change Active User======================================================

ChangeActiveUser::ChangeActiveUser(){}

void ChangeActiveUser::act(Session &sess) {
    std::string name;
    std::cin >> name;

    if (sess.findInUserMap(name) == nullptr)
       error("user does not exist.");
    else {
        activeUser = sess.findInUserMap(name);
        complete();
    }
    sess.addToActionsLog(this);
}

std::string ChangeActiveUser::toString() const {
    if (getStatus() == ERROR){
        return "ERROR: - " + getErrorMsg();
    }
    return "ChangeActiveUser " +  getStatus() ;
}
//=================================================Delete User==========================================================

DeleteUser::DeleteUser(User *user) : userToDelete(user) {}

void DeleteUser::act(Session &sess) {
    std::string name;
    std::cin >> name;

    if (sess.findInUserMap(name) == nullptr)
        error("User does not exist.");
    else {
        sess.deleteFromUserMap(userToDelete->getName());
        complete();
    }
    sess.addToActionsLog(this);
}

std::string DeleteUser::toString() const {
    if (getStatus() == ERROR){
        return "ERROR: - " + getErrorMsg();
    }
    return "CreateUser " +  getStatus() ;
}
}

//================================================Duplicate User========================================================
DuplicateUser::DuplicateUser(User *user, std::string newName):userToDuplicate(user), newName(newName){}


void DuplicateUser::act(Session &sess) {
    std::string name,thirdString;
    std::cin>> name;
    std::cin>> thirdString;

    if (sess.findInUserMap(name) == nullptr)
       error("User does not exist.");
    else if (sess.findInUserMap(thirdString) != nullptr)
        error("User name already taken");
    else {
        BaseAction* dup = new CreateUser(thirdString, userToDuplicate->getAlgo());
        User* newUser=sess.findInUserMap(thirdString);
        for (auto x: userToDuplicate->getHistory()) {
            newUser->addToHistory(x);
            complete();
        }
    sess.addToActionsLog(this);
    }
}

std::string DuplicateUser::toString() const {
    return std::__cxx11::string();
}

//=============================================Print Content List=======================================================

PrintContentList::PrintContentList()= default;

void PrintContentList::act(Session &sess) {
    for (int i=0; i<sess.getContent().size(); i++) {
        Watchable *print = sess.getContent().at(i);
        std::cout << print->getId()+1<< ". " << print->toString()<< " " << print->getLength()
        << " minutes" << " [" << print->tagsToString()+"]" << std::endl;
    }
}

std::string PrintContentList::toString() const {
    return "PrintContentList";
}

//=============================================Print Watch History======================================================

PrintWatchHistory::PrintWatchHistory()= default;

void PrintWatchHistory::act(Session &sess) {
    std::cout << "Watch history for " << sess.getActiveUser()->getName() << std::endl;
    for (int i=0; i<sess.getActiveUser()->getHistory().size();i++){
       std::cout << i+1 <<". " << sess.getActiveUser()->getHistory().at(i)->toString() << std::endl;
    }
}

std::string PrintWatchHistory::toString() const {
    return std::__cxx11::string();
}

//=====================================================Watch============================================================

Watch::Watch()=default ;

void Watch::act(Session &sess) {
    // if it a episode we need to rec the next one
    std::string answer="y";
    std::string stringid;
    std::cin >> stringid ;
    int intid = std::stoi(stringid) - 1 ;
    while (answer=="y"){

        Watchable* watched = sess.getContent().at(intid);
        std::cout << "Watching "<< watched->toString()<< std::endl ;
        sess.getActiveUser()->addToHistory(watched);

        std::cout << "We recommend watching " ;
        // If it's an episode and not the last episode of the series
        if (watched->isEpisode() &&
            watched->getSeriesName() ==  sess.getContent().at(intid+1)->getSeriesName())
        {
            std::cout << sess.getContent().at(intid+1)->toString();
            intid ++;
        }

        else{
            if (sess.getActiveUser()->getAlgo() == "len") {
                sess.getActiveUser()->updateRec(watched);
            }
            if (sess.getActiveUser()->getAlgo() == "rer") {
                sess.getActiveUser()->updateRec(watched);
            }
            if (sess.getActiveUser()->getAlgo() == "gen"){
                sess.getActiveUser()->updateRec(watched);
            }


            std::cout << sess.getActiveUser()->getRecommendation(sess)->toString();
            intid = sess.getActiveUser()->getRecommendation(sess)->getId();

        }
        std::cout << ", continue watching? [y/n]" << std::endl;
        std::cin >>  answer;
    }
}

std::string Watch::toString() const {
    return std::__cxx11::string();
}

//==============================================Print Actions Log=======================================================
PrintActionsLog::PrintActionsLog()= default;


void PrintActionsLog::act(Session &sess) {

}

std::string PrintActionsLog::toString() const {
    return std::__cxx11::string();
}

//=====================================================Exit=============================================================

void Exit::act(Session &sess) {

}

std::string Exit::toString() const {
    return std::__cxx11::string();
}
