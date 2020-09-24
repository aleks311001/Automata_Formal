//
// Created by aleks311001 on 22.09.2020.
//

#include "NKA.h"

NKA::NKA(long long q0,
         const std::set<char>& alphabet,
         const std::set<long long>& configurations,
         const std::set<long long>& acceptingConfigurations,
         const TransitionsType& transitions):
        configurations_(configurations),
        alphabet_(alphabet),
        transitions_(transitions),
        q0_(q0),
        acceptingConfigurations_(acceptingConfigurations) {}

NKA::NKA(const NKA& other):
        configurations_(other.configurations_),
        alphabet_(other.alphabet_),
        transitions_(other.transitions_),
        q0_(other.q0_),
        acceptingConfigurations_(other.acceptingConfigurations_) {}
NKA::NKA(NKA&& other):
        configurations_(std::move(other.configurations_)),
        alphabet_(std::move(other.alphabet_)),
        transitions_(std::move(other.transitions_)),
        q0_(std::move(other.q0_)),
        acceptingConfigurations_(std::move(other.acceptingConfigurations_)) {}

NKA& NKA::operator=(const NKA& other) {
    q0_ = other.q0_;
    alphabet_ = other.alphabet_;
    configurations_ = other.configurations_;
    acceptingConfigurations_ = other.acceptingConfigurations_;
    transitions_ = other.transitions_;

    return *this;
}
NKA& NKA::operator=(NKA&& other) {
    q0_ = other.q0_;
    alphabet_ = std::move(other.alphabet_);
    configurations_ = std::move(other.configurations_);
    acceptingConfigurations_ = std::move(other.acceptingConfigurations_);
    transitions_ = std::move(other.transitions_);

    return *this;
}

void NKA::changeQ0(long long q0) {
    q0_ = q0;
}

void NKA::addConfiguration(long long add) {
    if (configurations_.contains(add)) {
        throw std::invalid_argument("configurations already contains added argument");
    }
    configurations_.insert(add);
}
void NKA::addSymbol(char add) {
    if (configurations_.contains(add)) {
        throw std::invalid_argument("configurations already contains added argument");
    }
    alphabet_.insert(add);
}
void NKA::addTransition(long long left, const std::string& word, long long right) {
    if (!configurations_.contains(left) ||
        !configurations_.contains(right)) {
        throw std::invalid_argument("configurations does not contains added argument of transition");
    }

    for (char symbol: word) {
        if (!alphabet_.contains(symbol)) {
            throw std::invalid_argument("alphabet does not contains symbol of added word of transition");
        }
    }

    if (!transitions_.contains(left)) {
        transitions_.emplace(left, std::unordered_map<std::string, std::set<long long>>());
    }
    if (!transitions_[left].contains(word)) {
        transitions_[left].emplace(word, std::set<long long>());
    }
    if (!transitions_[left][word].contains(right)) {
        transitions_[left][word].insert(right);
    }
}
void NKA::addAcceptingConfiguration(long long add) {
    if (acceptingConfigurations_.contains(add)) {
        throw std::invalid_argument("accepting configurations already contains added argument");
    }
    acceptingConfigurations_.insert(add);
}
/*
void NKA::delConfiguration(long long del) {
    if (!configurations_.contains(del)) {
        throw std::invalid_argument("configurations does not contains deleted argument");
    }
    configurations_.erase(del);
}
void NKA::delSymbol(char del) {
    if (!alphabet_.contains(del)) {
        throw std::invalid_argument("alphabet does not contains deleted symbol");
    }
    alphabet_.erase(del);
}
void NKA::delTransition(long long left, const std::string& word, long long right) {
    if (!transitions_.contains(left) ||
        !transitions_[left].contains(word) ||
        !transitions_[left][word].contains(right)) {
        throw std::invalid_argument("transitions does not contains deleted transition");
    }

    transitions_[left][word].erase(right);
    if (transitions_[left][word].empty()) {
        transitions_[left].erase(word);
        if (transitions_[left].empty()) {
            transitions_.erase(left);
        }
    }
}
void NKA::delAcceptingConfiguration(long long del) {
    if (!configurations_.contains(del)) {
        throw std::invalid_argument("accepting configurations does not contains deleted configuration");
    }
    acceptingConfigurations_.erase(del);
}*/

long long NKA::addNewConfiguration(long long min) {
    for (long long i = min; ; ++i) {
        if (!configurations_.contains(i)) {
            addConfiguration(i);
            return i;
        }
    }
}

void NKA::replaceMultiSymbolsEdges() {
    for (auto& conf: configurations_) {
        if (transitions_.contains(conf)) {
            std::vector<std::string> badStrings;
            for (auto& pair: transitions_[conf]) {
                if (pair.first.size() > 1) {
                    badStrings.push_back(pair.first);
                    long long leftConf = conf;
                    long long rightConf = 0;
                    for (size_t i = 0; i < pair.first.size() - 1; ++i) {
                        rightConf = addNewConfiguration(rightConf);
                        addTransition(leftConf, std::string(&pair.first[i], 1), rightConf);
                        leftConf = rightConf;
                    }

                    for (auto& finalConf: pair.second) {
                        addTransition(leftConf, std::string(&pair.first.back(), 1), finalConf);
                    }
                }
            }

            for (auto& bad: badStrings) {
                transitions_[conf].erase(bad);
            }
        }
    }
}

void NKA::findAllWaysOnEpsEdges_(long long start,
                                 std::set<long long>& wasIn,
                                 std::list<std::pair<std::string, long long>>& endWayConfigurations,
                                 bool& hasAcceptingConf) {
    wasIn.insert(start);
    if (!transitions_.contains(start)) {
        return;
    }

    for (auto& pair: transitions_[start]) {
        if (pair.first == EPS) {
            for (auto& endConf: pair.second) {
                if (!hasAcceptingConf && acceptingConfigurations_.contains(endConf)) {
                    hasAcceptingConf = true;
                }
                if (!wasIn.contains(endConf)) {
                    findAllWaysOnEpsEdges_(endConf, wasIn, endWayConfigurations, hasAcceptingConf);
                }
            }
        } else {
            for (auto& endConf: pair.second) {
                endWayConfigurations.emplace_back(pair.first, endConf);
            }
        }
    }
}
void NKA::addTransitionsInEpsWays_(long long start, std::set<long long>& wasIn) {
    wasIn.insert(start);

    std::list<std::pair<std::string, long long>> ends;
    std::set<long long> wasInForFindEnds;
    bool hasAccept = false;
    findAllWaysOnEpsEdges_(start, wasInForFindEnds, ends, hasAccept);

    for (auto& pair: ends) {
        addTransition(start, pair.first, pair.second);
        if (!wasIn.contains(pair.second)) {
            addTransitionsInEpsWays_(pair.second, wasIn);
        }
    }

    if (hasAccept) {
        acceptingConfigurations_.insert(wasInForFindEnds.begin(), wasInForFindEnds.end());
    }
}
void NKA::removeEpsilonTransitions_() {
    for (auto& left: configurations_) {
        if (transitions_.contains(left)) {
            transitions_[left].erase(EPS);
        }
    }
}
void NKA::changeEpsTransitions() {
    std::set<long long> wasIn;
    addTransitionsInEpsWays_(q0_, wasIn);
    removeEpsilonTransitions_();
}

void NKA::delEmptyConfigurations() {
    std::set<long long> newConfigurations;
    for (auto& transition: transitions_) {
        if (!transitions_[transition.first].empty()) {
            newConfigurations.insert(transition.first);
        }
        for (auto& pair: transition.second) {
            newConfigurations.insert(pair.second.begin(), pair.second.end());
        }
    }

    configurations_ = std::move(newConfigurations);
    configurations_.insert(q0_);

    for (auto conf: acceptingConfigurations_) {
        if (!configurations_.contains(conf)) {
            acceptingConfigurations_.erase(conf);
        }
    }
}

std::unordered_map<long long, size_t> NKA::getMapConfigurationToNumber_() {
    std::unordered_map<long long, size_t> result;

    int i = 1;
    for (auto conf: configurations_) {
        if (q0_ == conf) {
            result.emplace(conf, 0);
        } else {
            result.emplace(conf, i);
            ++i;
        }
    }

    return result;
}
long long NKA::makeConfigurationFromOthers_(const std::set<long long int>& configurations,
                                            const std::unordered_map<long long int, size_t>& indexesConfigs) {
    long long result = 0;

    for (auto& conf: configurations) {
        result |= (1ll<<indexesConfigs.at(conf));
    }

    return result;
}
bool NKA::checkSetConfigsOnAccepting_(const std::set<long long int>& configurations) {
    for (auto conf: configurations) {
        if (acceptingConfigurations_.contains(conf)) {
            return true;
        }
    }

    return false;
}

void NKA::makeExplicitWays() {
    if (configurations_.size() > std::numeric_limits<long long>::digits) {
        delEmptyConfigurations();
        if (configurations_.size() > std::numeric_limits<long long>::digits) {
            throw std::range_error("number of configurations over max");
        }
    }

    std::unordered_map<long long, size_t> numConfigurations = getMapConfigurationToNumber_();

    long long newConf = makeConfigurationFromOthers_(std::set<long long>({q0_}), numConfigurations);
    NKA newNKA (newConf, alphabet_);
    newNKA.addConfiguration(newConf);
    if (acceptingConfigurations_.contains(q0_)) {
        newNKA.addAcceptingConfiguration(newConf);
    }

    std::queue<std::pair<long long, std::set<long long>>> configurationsQueue;
    configurationsQueue.emplace(newConf, std::set<long long>({q0_}));

    while (!configurationsQueue.empty()) {
        auto nowConf = configurationsQueue.front();
        configurationsQueue.pop();

        std::unordered_map<std::string, std::set<long long>> newTransition;
        for (auto& conf: nowConf.second) {
            for (auto& pair: transitions_[conf]) {
                if (!newTransition.contains(pair.first)) {
                    newTransition.emplace(pair);
                } else {
                    newTransition[pair.first].insert(pair.second.begin(), pair.second.end());
                }
            }
        }

        for (auto& pair: newTransition) {
            newConf = makeConfigurationFromOthers_(pair.second, numConfigurations);

            if (!newNKA.configurations_.contains(newConf)) {
                newNKA.addConfiguration(newConf);
                configurationsQueue.emplace(newConf, pair.second);
            }

            if (!newNKA.acceptingConfigurations_.contains(newConf) && checkSetConfigsOnAccepting_(pair.second)) {
                newNKA.addAcceptingConfiguration(newConf);
            }

            newNKA.addTransition(nowConf.first, pair.first, newConf);
        }
    }

    *this = newNKA;
}
void NKA::makeDKA() {
    replaceMultiSymbolsEdges();
    changeEpsTransitions();
    makeExplicitWays();
}

void NKA::makeFullDKAFromDKA() {
    delEmptyConfigurations();
    long long newConf = addNewConfiguration();

    for (auto& conf: configurations_) {
        if (conf != newConf) {
            for (auto& symbol: alphabet_) {
                auto stringSym = std::string(&symbol, 1);
                if (!transitions_[conf].contains(stringSym)) {
                    addTransition(conf, stringSym, newConf);
                }
            }
        }
    }
}

void NKA::makeAntiDKAFromFullDKA() {
    std::set<long long> antiAccepting;

    for (auto& conf: configurations_) {
        if (!acceptingConfigurations_.contains(conf)) {
            antiAccepting.insert(conf);
        }
    }

    acceptingConfigurations_ = std::move(antiAccepting);
}


void NKA::writeConfigurations_(std::ofstream& file,
                               std::unordered_map<long long, size_t>& numsConfigurations,
                               int r) {
    for (auto& conf: configurations_) {
        file << "    \\node [state";
        if (conf == q0_) {
            file << ", initial";
        }
        if (acceptingConfigurations_.contains(conf)) {
            file << ", accepting";
        }

        double angle = 2 * M_PI / configurations_.size() * numsConfigurations[conf] + M_PI;
        file << "] (" << numsConfigurations[conf] << ") at ("
                << r * std::cos(angle) << ", "
                << r * std::sin(angle) << ") {"
                << numsConfigurations[conf] << "};\n";
    }
}
void NKA::writeEdge_(std::ofstream& file, std::string word,
                     long long int startConf, long long int finishConf,
                     std::unordered_map<long long, std::set<long long>>& daddies,
                     std::unordered_map<long long int, size_t>& numsConfigurations) {
    file << "\n            edge ";

    if (finishConf == startConf) {
        file << "[loop ";

        int sz = numsConfigurations.size();
        int num = numsConfigurations[finishConf];
        if (num > sz / 8 && num <= sz * 3 / 8) {
            file << "below] ";
        } else if ((num > sz * 3 / 8 && num <= sz * 5 / 8) || num == 0) {
            file << "right] ";
        } else if (num > sz * 5 / 8 && num <= sz * 7 / 8) {
            file << "above] ";
        } else {
            file << "left] ";
        }
    } else {
        if (daddies[startConf].contains(finishConf)) {
            file << "[bend left = 20] ";
        }
    }

    file << "node {$" << word;
    if (word == EPS) {
        file << "\\varepsilon";
    }
    file << "$} (" << numsConfigurations[finishConf] << ")";
}
std::unordered_map<long long, std::set<long long>> NKA::findDaddies() {
    std::unordered_map<long long, std::set<long long>> result;

    for (auto& conf: transitions_) {
        for (auto& pair: conf.second) {
            for (auto& finish: pair.second) {
                if (result.contains(finish)) {
                    result[finish].insert(conf.first);
                } else {
                    result.emplace(finish, std::set<long long>{conf.first});
                }
            }
        }
    }

    return result;
}
void NKA::writeTransitions_(std::ofstream& file,
                            std::unordered_map<long long, size_t>& numsConfigurations) {
    auto daddies = findDaddies();
    file << "\n    \\path";
    for (auto& transition: transitions_) {
        file << "\n        ("
             << numsConfigurations[transition.first] << ") ";

        for (auto& pair: transition.second) {
            for (auto& conf: pair.second) {
                writeEdge_(file, pair.first, transition.first, conf, daddies, numsConfigurations);
            }
        }
    }
    file << ";\n";
}
void NKA::createTexFileThisNKA(const std::string& filename, double r, bool writeRegular) {
    NKA copy(*this);
    alphabet_.insert('+');
    makeOneEdgeForAllPairs_();

    std::ofstream newFile(filename);
    std::ifstream prefix("PrefixTexFile.txt");
    newFile << prefix.rdbuf();
    prefix.close();

    auto numsConfigurations = getMapConfigurationToNumber_();
    writeConfigurations_(newFile, numsConfigurations, r);
    writeTransitions_(newFile, numsConfigurations);

    newFile << "\\end{tikzpicture}\n\n";
    if (writeRegular) {
        newFile << "$" + makeRegular() + "$\n";
    }
    newFile << "\\end{document}\n";

    *this = std::move(copy);
}


void NKA::makeOneAcceptingConfiguration_() {
    long long newAcc = addNewConfiguration();

    for (auto& conf: acceptingConfigurations_) {
        addTransition(conf, EPS, newAcc);
    }

    acceptingConfigurations_.clear();
    acceptingConfigurations_.insert(newAcc);
}
void NKA::replaceEpsilonToOne_() {
    for (auto& conf: configurations_) {
        if (transitions_.contains(conf)) {
            if (transitions_[conf].contains(EPS)) {
                transitions_[conf].emplace("1", std::move(transitions_[conf][EPS]));
                transitions_[conf].erase(EPS);
            }
        }
    }
}
void NKA::makeOneEdgeForAllPairs_() {
    for (auto& conf: configurations_) {
        if (transitions_.contains(conf)) {
            std::unordered_map<long long, std::string> configurationToEdge;

            for (auto& pair: transitions_[conf]) {
                for (auto& finalConf: pair.second) {
                    if (configurationToEdge.contains(finalConf)) {
                        configurationToEdge[finalConf] += "+" + pair.first;
                    } else {
                        configurationToEdge.emplace(finalConf, pair.first);
                    }
                }
            }

            transitions_[conf].clear();
            for (auto& pair: configurationToEdge) {
                addTransition(conf, pair.second, pair.first);
            }
        }
    }
}
/*
std::unordered_map<long long, long long> NKA::getMapFinalEdgeToStart_() {
    std::unordered_map<long long, long long> result;

    for (auto& conf: configurations_) {
        if (transitions_.contains(conf)) {
            for (auto& pair: transitions_[conf]) {
                for (auto& final: pair.second) {
                    result.emplace(final, conf);
                }
            }
        }
    }

    return result;
}*/

std::string NKA::getBracketedExpression_(const std::string& string) {
    if (string.size() > 1) {
        return "(" + string + ")";
    }
    if (string == "1") {
        return EPS;
    }
    return string;
}
std::string NKA::getStarsExpression_(const std::string& string) {
    auto bracket = getBracketedExpression_(string);
    if (bracket == "1" || bracket == EPS) {
        return bracket;
    }
    return bracket += "^*";
}

std::string NKA::unionEdgeLoopEdge(const std::string& left, const std::string& loop, const std::string& right) {
    std::string result = getBracketedExpression_(left) + getStarsExpression_(loop) + getBracketedExpression_(right);
    if (result.empty()) {
        return "1";
    }
    return result;
}
void NKA::skipConfiguration_(long long int conf) {
    for (auto& start: configurations_) {
        if (start != conf) {
            for (auto& pairLeft: transitions_[start]) {
                if (pairLeft.second.contains(conf)) {
                    std::string middle;
                    for (auto& pairRight: transitions_[conf]) {
                        for (auto& finish: pairRight.second) {
                            if (finish == conf) {
                                middle = pairRight.first;
                            }
                        }
                    }

                    for (auto& pairRight: transitions_[conf]) {
                        for (auto& finish: pairRight.second) {
                            if (finish != conf) {
                                std::string transit = unionEdgeLoopEdge(pairLeft.first, middle, pairRight.first);
                                addTransition(start, transit, finish);
                            }
                        }
                    }

                    pairLeft.second.erase(conf);
                    if (pairLeft.second.empty()) {
                        transitions_[start].erase(pairLeft.first);
                    }
                    break;
                }
            }
        }
    }

    transitions_.erase(conf);
}

std::string NKA::makeRegular() {
    NKA copy = *this;

    addRegularSymbols();

    delEmptyConfigurations();
    makeOneAcceptingConfiguration_();
    replaceEpsilonToOne_();

    for (auto& conf: configurations_) {
        if (conf != q0_ && !acceptingConfigurations_.contains(conf) && transitions_.contains(conf)) {
            makeOneEdgeForAllPairs_();
            skipConfiguration_(conf);
            //createTexFileThisNKA("debug.tex", 5, false);
        }
    }

    makeOneEdgeForAllPairs_();
    if (acceptingConfigurations_.contains(q0_)) {
        for (auto& pair: transitions_[q0_]) {
            if (pair.second.contains(q0_)) {
                return getStarsExpression_(pair.first);
            }
        }
    }

    std::string q0_q0, q0_ac, ac_q0, ac_ac;
    long long ac = *acceptingConfigurations_.begin();
    for (auto& pair: transitions_[q0_]) {
        if (pair.second.contains(q0_)) {
            q0_q0 = getStarsExpression_(pair.first);
        }
        if (pair.second.contains(ac)) {
            q0_ac = getBracketedExpression_(pair.first);
        }
    }

    for (auto& pair: transitions_[ac]) {
        if (pair.second.contains(q0_)) {
            ac_q0 = getBracketedExpression_(pair.first);
        }
        if (pair.second.contains(ac)) {
            if (!pair.first.empty()) {
                ac_ac = pair.first + "+";
            }
        }
    }

    *this = std::move(copy);
    return q0_q0 + q0_ac + getStarsExpression_(ac_ac + ac_q0 + q0_q0 + q0_ac);
}
