#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

using namespace std;

BaseAction::BaseAction() {
    status= PENDING;
}

//============================================Action Class Methods======================================================

/*
 * Returns the status of the current action
 * @return ActionStatus PENDING, COMPLETED, ERROR based on the status.
 * (default)
 */
ActionStatus BaseAction::getStatus() const {
    return status;
}

/*
 * Updates the status of the action to COMPLETED.
 * (default)
 */
void BaseAction::complete() {
    status = COMPLETED;
}

/*
 * Updates the status of the action to ERROR and update the error massage.
 * (default)
 */
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

/*
 * Return string of the relevant error massage.
 * (default)
 */
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

//=================================================Create User==========================================================
// TODO: think if we need to keep the constructors.
CreateUser::CreateUser() {}

/*
 * Start the createUser action. Create new user based on the input algorithm,
 * and add the new user to the userMap.
 * @param: Session &sess: reference to Session object.
 * (default)
 */
void CreateUser::act(Session &sess) {
    User* user;
    //std::string name, thirdString;
    //std::cin >> name;
    //std::cin >> thirdString;
    std::string userName = sess.getUserName();
    std::string userAlgo = sess.getThirdParameter();
    if ((userAlgo != "len") & (userAlgo != "rer") & (userAlgo != "gen")) {
        // actionsLog.push_back()
        error("Invalid algorithm input.");
        std::cout<< "error" << getErrorMsg();
    }
    else if (sess.findInUserMap(userName) != nullptr) {
       error("User already exist.");
        std::cout<< "error" << getErrorMsg();
    }
    else if (userAlgo == "len") {
        user = new LengthRecommenderUser(userName);
        complete();
    }
    else if (userAlgo == "rer") {
        user = new RerunRecommenderUser(userName);
        complete();
    }
    else if (userAlgo == "gen") {
        user = new GenreRecommenderUser(userName);
        complete();
    }
    sess.addToUserMap(userName , user);
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string CreateUser::toString() const {
    std::string str;
    if (getStatus() == ERROR){
        str =  "ERROR: - " + getErrorMsg();
    }
//    else {
//        // TODO find a way to case an UNUM to string
//        str = "CreateUser " + getStatus();
//    }
    return str;
}

//==============================================Change Active User======================================================

ChangeActiveUser::ChangeActiveUser(){}

/*
 * Start the ChangeActiveUser action. Change the active user to the input user.
 * (default)
 */
void ChangeActiveUser::act(Session &sess) {
    std::string userName;
    userName = sess.getUserName();
//    std::cin >> name;

    if (sess.findInUserMap(userName) == nullptr)
       error("user does not exist.");
    else {
        sess.setActiveUser(sess.findInUserMap(userName));
        complete();
    }
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string ChangeActiveUser::toString() const {
    std::string str;
    if (getStatus() == ERROR){
        str =  "ERROR: - " + getErrorMsg();
    }
//    else {
//        // TODO find a way to cast an UNUM to string
//        str = "ChangeActiveUser " + getStatus();
//    }
    return str;
}
//=================================================Delete User==========================================================

DeleteUser::DeleteUser() {}

/*
 * Start the DeleteUser action.
 * Deleted the user from the userMap.
 * (default)
 */
void DeleteUser::act(Session &sess) {
    std::string userName;
    userName = sess.getUserName();
    //std::cin >> name;

    if (sess.findInUserMap(userName) == nullptr)
        error("User does not exist.");
    else {
        sess.deleteFromUserMap(userName);
        complete();
    }
    sess.addToActionsLog(this);
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string DeleteUser::toString() const {
    std::string str;
    if (getStatus() == ERROR){
        str =  "ERROR: - " + getErrorMsg();
    }
//    else {
//        // TODO find a way to case an UNUM to string
//        str = "DeleteUser " + getStatus();
//    }
    return str;
}


//================================================Duplicate User========================================================
DuplicateUser::DuplicateUser() {}

/*
 * Start the DuplicateUser action.
 * Creates new user with new given name, but with same algorithm and watching history
 * of exist user.
 * (default)
 */
void DuplicateUser::act(Session &sess) {
    std::string userName, newUserName;
    userName = sess.getUserName();
    newUserName = sess.getThirdParameter();
//    std::cin>> name;
//    std::cin>> thirdString;

    if (sess.findInUserMap(userName) == nullptr)
       error("User does not exist.");
    else if (sess.findInUserMap(newUserName) != nullptr)
        error("User name already taken");
    else {
        User* newUser=sess.findInUserMap(newUserName);
        BaseAction* dup = new CreateUser();
        for (auto x: userToDuplicate->getHistory()) {
            newUser->addToHistory(x);
            complete();
        }
    sess.addToActionsLog(this);
    }
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string DuplicateUser::toString() const {
    std::string str;
    if (getStatus() == ERROR){
        str =  "ERROR: - " + getErrorMsg();
    }
//    else {
//        // TODO find a way to case an UNUM to string
//        str = "DuplicateUser " + getStatus();
//    }
    return str;
}

//=============================================Print Content List=======================================================

PrintContentList::PrintContentList()= default;

/*
 * Start the PrintContentList action.
 * Prints all the available content to the active user.
 * (default)
 */
void PrintContentList::act(Session &sess) {
    for (int i=0; i<sess.getContent().size(); i++) {
        Watchable *print = sess.getContent().at(i);
        std::cout << print->getId()+1<< ". " << print->toString()<< " " << print->getLength()
        << " minutes" << " [" << print->tagsToString()+"]" << std::endl;
    }
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string PrintContentList::toString() const {
    return "PrintContentList";
}

//=============================================Print Watch History======================================================

PrintWatchHistory::PrintWatchHistory()= default;

/*
 * Start the PrintWatchHistory action.
 * Prints the watch history of the active user.
 * (default)
 */
void PrintWatchHistory::act(Session &sess) {
    std::cout << "Watch history for " << sess.getActiveUser()->getName() << std::endl;
    for (int i=0; i<sess.getActiveUser()->getHistory().size();i++){
       std::cout << i+1 <<". " << sess.getActiveUser()->getHistory().at(i)->toString() << std::endl;
    }
}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string PrintWatchHistory::toString() const {
    return std::string();
}

//=====================================================Watch============================================================

Watch::Watch()=default ;

/*
 * Start the Watch action.
 * Watching content (movie/episode) based on given content id or reccomendation.
 * (default)
 */
void Watch::act(Session &sess) {
    // Todo: Implement getNextWatchable using visitor pattern
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

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string Watch::toString() const {
    return std::string();
}

//==============================================Print Actions Log=======================================================
PrintActionsLog::PrintActionsLog()= default;

/*
 * Start the PrintActionLog action.
 * Prints all the actions that were performed in the current session with their statuses and messages.
 * (default)
 */
void PrintActionsLog::act(Session &sess) {

}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string PrintActionsLog::toString() const {
    return std::string();
}

//=====================================================Exit=============================================================
// TODO: what should we do here?
/*
 * Start the Exit action.
 */
void Exit::act(Session &sess) {

}

/*
 * Returns string with information about the action status.
 * (default)
 */
std::string Exit::toString() const {
    return std::string();
}
