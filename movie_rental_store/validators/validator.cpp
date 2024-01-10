#include "validator.h"
#include <cassert>
#include <sstream>

using std::stringstream;


void MovieValidator::validate(const Movie &m) {
    vector <string> msgs;
    if (m.getTitle().empty())
        msgs.push_back("Titlu vid!!!");
    if (m.getGenre().empty())
        msgs.push_back("Gen vid!!!");
    if (m.getActor().empty())
        msgs.push_back("Actor vid!!!");

    if (m.getYear() <= 1900)
        msgs.push_back("An invalid!!!");
    if (!msgs.empty()) {
        throw ValidateException(msgs);
    }
}


ostream& operator <<(ostream& out, const ValidateException& ex) {
    for (const auto& msg : ex.msgs) {
        out << msg << " ";
    }
    return out;
}

string ValidateException::getMsgs() {
    string fullMsg = "";
    for (const auto& msg : msgs) {
        fullMsg += msg + "\n";
    }
    return fullMsg;
}


void testValidator() {
    MovieValidator val;

    Movie m{"", "", "", 1800};
    try {
        val.validate(m);
    }
    catch (const ValidateException& ex) {
        stringstream out;
        out << ex;
        auto msg = out.str();
        assert(msg.find("negativ") >= 0);
        assert(msg.find("vid") >= 0);
    }
}