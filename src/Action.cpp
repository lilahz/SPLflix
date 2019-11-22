//
// Created by lilachzi@wincs.cs.bgu.ac.il on 21/11/2019.
//

#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

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


    //TODO adding the history.
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

DuplicateUser::DuplicateUser(User *user, std::string newName):userToDuplicate(user), newName(newName){
    user->setHistory(userToDuplicate->get_history());
    user->setNotSeen(userToDuplicate->newget_notSeen()); /// is this the right place>?!
}


void DuplicateUser::act(Session &sess) {
    BaseAction* newUser =new CreateUser(newName, userToDuplicate->getAlgo());
    newUser->act(sess);





    //TODO adding the history and not seen.
}

std::string DuplicateUser::toString() const {
    return std::__cxx11::string();
}

PrintContentList::PrintContentList()= default;

void PrintContentList::act(Session &sess) {

    for (int i=0;i<sess.getContent().size();i++)
    {
        Watchable *print = sess.getContent().at(i);
        std::cout << print->getId()+1<< ". " << print->toString()<< " " << print->getLength()
        << " minutes" << " [" << print->getTags()+"]" << std::endl;

    }
}

std::string PrintContentList::toString() const {
    return "PrintContentList";
}

PrintWatchHistory::PrintWatchHistory()= default;


void PrintWatchHistory::act(Session &sess) {
    std::cout <<sess.getAcitveUser()->getName();
    for (int i=0; i<sess.getAcitveUser()->get_history().size();i++){
       std::cout <<"i. " << sess.getAcitveUser()->get_history().at(i)->toString() << std::endl;
    }

}

std::string PrintWatchHistory::toString() const {
    return std::__cxx11::string();
}

Watch::Watch()=default ;

void Watch::act(Session &sess) {
    std::string id;
    std::cin >> id;
    int id2 = std::stoi(id);
    Watchable* watched = sess.getContent().at(id2);
    std::cout << "Watching "<< watched->toString()<< std::endl ;
    sess.getAcitveUser()->get_history().push_back(watched);
    //std::cout<< sess.getAcitveUser()->get_history().at(0)->toString();
    std::cout<< "get stoned" <<std::endl; //it should be sent to rec!

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
