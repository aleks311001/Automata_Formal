//
// Created by aleks311001 on 22.09.2020.
//

#include "NKA.h"


bool equalMaps(const std::unordered_map<long long, long long>& left,
               const std::unordered_map<long long, long long>& right) {
    if (left.size() != right.size()) {
        return false;
    }

    for (auto& pair: left) {
        if (!right.contains(pair.first)) {
            return false;
        }

        if (right.at(pair.first) != pair.second) {
            return false;
        }
    }

    return true;
}

bool equalVectors(const std::vector<long long>& left,
                  const std::vector<long long>& right) {
    if (left.size() != right.size()) {
        return false;
    }

    for (size_t i = 0; i < left.size(); ++i) {
        if (left[i] != right[i]) {
            return false;
        }
    }

    return true;
}


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

NKA::NKA(long long q0,
         const std::set<char>& alphabet,
         long long numConfigurations,
         const std::set<long long int>& acceptingConfigurations,
         const NKA::TransitionsType& transitions):
         NKA(q0, alphabet, std::set<long long>(), acceptingConfigurations, transitions) {
    for (int i = 0; i < numConfigurations; ++i) {
        configurations_.insert(i);
    }
}

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
void NKA::addTransitionWithoutCheck_(long long int left, const std::string& word, long long int right) {
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

    addTransitionWithoutCheck_(left, word, right);
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
}*/
void NKA::delAcceptingConfiguration(long long del) {
    if (!configurations_.contains(del)) {
        throw std::invalid_argument("accepting configurations does not contains deleted configuration");
    }
    acceptingConfigurations_.erase(del);
}

void NKA::delConfiguration(long long conf) {
    configurations_.erase(conf);
    acceptingConfigurations_.erase(conf);
    transitions_.erase(conf);

    for (auto& transition: transitions_) {
        for (auto& pair: transition.second) {
            if (pair.second.contains(conf)) {
                pair.second.erase(conf);
            }
        }
    }
}

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

/*bool NKA::hasWayToAccepting_(long long start,
                             std::unordered_map<long long, bool>& newConfigurations,
                             std::unordered_map<long long, bool>& wasIn) {

    if (wasIn[start]) {
        if (newConfigurations[start]) {
            return true;
        } else {
            return false;
        }
    }

    wasIn[start] = true;

    if (acceptingConfigurations_.contains(start)) {
        newConfigurations[start] = true;
        return true;
    }

    if (!transitions_.contains(start)) {
        return false;
    }

    for (auto& pair: transitions_[start]) {
        for (auto& conf: pair.second) {
            if (hasWayToAccepting_(conf, newConfigurations, wasIn)) {
                newConfigurations[start] = true;
            }
        }
    }

    return newConfigurations[start];
}*/
void NKA::delEmptyConfigurations() {
    std::set<long long> newConfigurations;
    std::queue<long long> qConfigurations;
    qConfigurations.push(q0_);
    newConfigurations.insert(q0_);

    while (!qConfigurations.empty()) {
        long long now = qConfigurations.front();
        qConfigurations.pop();

        if (!transitions_.contains(now)) {
            break;
        }
        for (auto& pair: transitions_[now]) {
            for (auto& conf: pair.second) {
                if (!newConfigurations.contains(conf)) {
                    qConfigurations.push(conf);
                    newConfigurations.insert(conf);
                }
            }
        }
    }

    for (auto& conf: configurations_) {
        if (!newConfigurations.contains(conf)) {
            if (acceptingConfigurations_.contains(conf)) {
                acceptingConfigurations_.erase(conf);
            }
            if (transitions_.contains(conf)) {
                transitions_.erase(conf);
            }
        }
    }

    configurations_ = std::move(newConfigurations);

    /*std::unordered_map<long long, bool> newConfigurations;
    std::unordered_map<long long, bool> wasIn;

    for (auto& conf: configurations_) {
        newConfigurations.emplace(conf, false);
        wasIn.emplace(conf, false);
    }

    hasWayToAccepting_(q0_, newConfigurations, wasIn);

    for (auto& pair: newConfigurations) {
        if (!pair.second) {
            delConfiguration(pair.first);
        }
    }*/
    /*std::set<long long> newConfigurations;
    auto daddies = findDaddies();
    std::queue<long long> qConfigurations;

    for (auto& conf: acceptingConfigurations_) {
        newConfigurations.insert(conf);
        qConfigurations.push(conf);
    }

    while (!qConfigurations.empty()) {
        long long nowConf = qConfigurations.front();
        qConfigurations.pop();

        for (auto& conf: daddies[nowConf]) {
            if (!newConfigurations.contains(conf)) {
                newConfigurations.insert(conf);
                qConfigurations.push(conf);
            }
        }
    }
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

    std::set<long long> newAcceptingConfigurations;
    for (auto conf: acceptingConfigurations_) {
        if (configurations_.contains(conf)) {
            newAcceptingConfigurations.insert(conf);
        }
    }
    acceptingConfigurations_ = std::move(newAcceptingConfigurations);*/
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
    unsigned long long result = 0;

    for (auto& conf: configurations) {
        result |= (1ull<<indexesConfigs.at(conf));
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
    if (configurations_.size() > std::numeric_limits<long long>::digits - 1) {
        delEmptyConfigurations();
        if (configurations_.size() > std::numeric_limits<long long>::digits - 1) {
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
                if (!transitions_[conf].contains(stringSym) || transitions_[conf][stringSym].empty()) {
                    addTransition(conf, stringSym, newConf);
                }
            }
        }
    }

    for (auto& symbol: alphabet_) {
        addTransition(newConf, std::string(&symbol, 1), newConf);
    }
}
void NKA::makeFullDKA() {
    makeDKA();
    makeFullDKAFromDKA();
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

void NKA::makeAntiDKA() {
    makeFullDKA();
    makeAntiDKAFromFullDKA();
    delEmptyConfigurations();
}

void NKA::writePrefix_(std::ofstream& file)  {
    std::ifstream prefix("PrefixTexFile.txt");
    file << prefix.rdbuf();
    prefix.close();
}
void NKA::writeAlphabet_(std::ofstream& file) {
    file << "    $\\Sigma = \\left\\{";
    size_t i = 0;
    for (auto& symbol: alphabet_) {
        file << symbol;
        if (i != alphabet_.size() - 1) {
            file << ", ";
        }
        ++i;
    }
    file << "\\right\\}$\\\\\n";
}
void NKA::writeAcceptingConfigurations_(std::ofstream& file,
                                       const std::unordered_map<long long int, size_t>& numsConfigurations) {
    file << "    $F = \\left\\{";
    size_t i = 0;
    for (auto& configuration: acceptingConfigurations_) {
        file << numsConfigurations.at(configuration);
        if (i != acceptingConfigurations_.size() - 1) {
            file << ", ";
        }
        ++i;
    }
    file << "\\right\\}$\n\n";
}

void NKA::writeConfigurations_(std::ofstream& file,
                               const std::unordered_map<long long, size_t>& numsConfigurations,
                               double r) {
    for (auto& conf: configurations_) {
        file << "    \\node [state";
        if (conf == q0_) {
            file << ", initial";
        }
        if (acceptingConfigurations_.contains(conf)) {
            file << ", accepting";
        }

        double angle = 2 * M_PI / configurations_.size() * numsConfigurations.at(conf) + M_PI;
        file << "] (" << numsConfigurations.at(conf) << ") at ("
                << r * std::cos(angle) << ", "
                << r * std::sin(angle) << ") {"
                << numsConfigurations.at(conf) << "};\n";
    }
}
void NKA::writeEdge_(std::ofstream& file, std::string word,
                     long long int startConf, long long int finishConf, /*int angle,*/
                     const std::unordered_map<long long, std::set<long long>>& daddies,
                     const std::unordered_map<long long int, size_t>& numsConfigurations) {
    file << "\n            edge ";

    if (finishConf == startConf) {
        file << "[loop ";

        int sz = numsConfigurations.size();
        int num = numsConfigurations.at(finishConf);
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
        if (daddies.at(startConf).contains(finishConf)) {
            file << "[bend left = 20] ";
        }
    }

    file << "node {$" << word;
    if (word == EPS) {
        file << "\\varepsilon";
    }
    file << "$} (" << numsConfigurations.at(finishConf) << ")";
}
std::unordered_map<long long, std::set<long long>> NKA::findDaddies() {
    std::unordered_map<long long, std::set<long long>> result;

    for (auto& conf: configurations_) {
        result.emplace(conf, std::set<long long>());
    }

    for (auto& conf: transitions_) {
        for (auto& pair: conf.second) {
            for (auto& finish: pair.second) {
                result[finish].insert(conf.first);
            }
        }
    }

    return result;
}
void NKA::writeTransitions_(std::ofstream& file,
                            std::unordered_map<long long, size_t>& numsConfigurations) {
    auto daddies = findDaddies();
    //std::unordered_map<long long, std::unordered_map<long long, int>> numEdges;

    file << "\n    \\path";

    for (auto& transition: transitions_) {
        file << "\n        ("
             << numsConfigurations[transition.first] << ") ";

        for (auto& pair: transition.second) {
            for (auto& conf: pair.second) {
                //++numEdges[transition.first][conf];
                writeEdge_(file, pair.first, transition.first, conf,/* numEdges[transition.first][conf] * 10,*/
                           daddies, numsConfigurations);
            }
        }
    }
    file << ";\n";
}
void NKA::writeMainPart_(std::ofstream& file, double r) {
    auto numsConfigurations = getMapConfigurationToNumber_();
    writeAlphabet_(file);
    file << "    $q_0 = " << numsConfigurations.at(q0_) << "$\\\\\n";
    writeAcceptingConfigurations_(file, numsConfigurations);

    makeOneEdgeForAllPairs_(',', "\\varepsilon");

    file << "\\begin{tikzpicture}"
               "[->, >=stealth', on grid, auto, node distance = 2.5cm, every state/.style={thick, fill=gray!20}]";
    writeConfigurations_(file, numsConfigurations, r);
    writeTransitions_(file, numsConfigurations);
}
void NKA::writeSuffix_(std::ofstream& file, bool writeRegular) {
    file << "\\end{tikzpicture}\n\n";
    if (writeRegular) {
        file << "$" + makeRegular() + "$\n";
    }
    file << "\\end{document}\n";
}
void NKA::createTexFileThisNKAWithoutCopy(const std::string& filename, double r) {
    std::ofstream newFile(filename);

    writePrefix_(newFile);
    writeMainPart_(newFile, r);
    writeSuffix_(newFile, false);

    newFile.close();
}
void NKA::createTexFileThisNKA(const std::string& filename, double r, bool writeRegular) {
    NKA copy = *this;
    std::ofstream newFile(filename);

    copy.writePrefix_(newFile);
    copy.writeMainPart_(newFile, r);
    NKA(*this).writeSuffix_(newFile, writeRegular);

    newFile.close();
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
void NKA::makeOneEdgeForAllPairs_(char symbolBetweenWords, const std::string& replaceEPS) {
    for (auto& conf: configurations_) {
        if (transitions_.contains(conf)) {
            std::unordered_map<long long, std::string> configurationToEdge;

            for (auto& pair: transitions_[conf]) {
                for (auto& finalConf: pair.second) {
                    if (configurationToEdge.contains(finalConf)) {
                        if (pair.first == EPS) {
                            (configurationToEdge[finalConf] += symbolBetweenWords) += replaceEPS;
                        }
                        configurationToEdge[finalConf] += symbolBetweenWords + pair.first;
                    } else {
                        if (pair.first == EPS) {
                            configurationToEdge.emplace(finalConf, replaceEPS);
                        }
                        configurationToEdge.emplace(finalConf, pair.first);
                    }
                }
            }

            transitions_[conf].clear();
            for (auto& pair: configurationToEdge) {
                addTransitionWithoutCheck_(conf, pair.second, pair.first);
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

std::string NKA::getRegularIfOneConfiguration_(long long int conf) {
    for (auto& pair: transitions_[conf]) {
        if (pair.second.contains(conf)) {
            return getStarsExpression_(pair.first);
        }
    }
    return "";
}
std::string NKA::getRegularIfTwoConfiguration_(long long int start, long long int accept) {
    std::string st_st, st_ac, ac_st, ac_ac;
    bool hasAc_st = false;
    bool hasSt_ac = false;

    for (auto& pair: transitions_[start]) {
        if (pair.second.contains(start)) {
            st_st = getStarsExpression_(pair.first);
        }
        if (pair.second.contains(accept)) {
            st_ac = getBracketedExpression_(pair.first);
            hasSt_ac = true;
        }
    }

    for (auto& pair: transitions_[accept]) {
        if (pair.second.contains(start)) {
            ac_st = getBracketedExpression_(pair.first);
            hasAc_st = true;
        }
        if (pair.second.contains(accept)) {
            if (!pair.first.empty()) {
                ac_ac = pair.first + "+";
            }
        }
    }

    if (!hasSt_ac) {
        return "\\emptyset";
    }
    if (!hasAc_st) {
        return st_st + st_ac + getStarsExpression_(ac_ac);
    }
    return st_st + st_ac + getStarsExpression_(ac_ac + ac_st + st_st + st_ac);
}
void NKA::addRegularSymbols() {
    alphabet_.insert('+');
    alphabet_.insert('*');
    alphabet_.insert('(');
    alphabet_.insert(')');
    alphabet_.insert('1');
    alphabet_.insert('^');
}
std::string NKA::makeRegularWithoutCopy() {
    NKA copy = *this;

    addRegularSymbols();

    delEmptyConfigurations();
    makeOneAcceptingConfiguration_();
    replaceEpsilonToOne_();

    for (auto& conf: configurations_) {
        //createTexFileThisNKA("tests/stepMakeRegularFor" + std::to_string(conf) + ".tex", 5, false);
        if (conf != q0_ && !acceptingConfigurations_.contains(conf) && transitions_.contains(conf)) {
            makeOneEdgeForAllPairs_('+', "1");
            skipConfiguration_(conf);
        }
    }
    //createTexFileThisNKA("tests/stepMakeRegularForLast.tex", 5, false);

    makeOneEdgeForAllPairs_();

    std::string result;

    if (acceptingConfigurations_.contains(q0_)) {
        result = getRegularIfOneConfiguration_(q0_);
    } else {
        long long ac = *acceptingConfigurations_.begin();

        result = getRegularIfTwoConfiguration_(q0_, ac);
    }

    return result;
}
std::string NKA::makeRegular() {
    NKA copy = *this;
    return copy.makeRegularWithoutCopy();
}

void NKA::makeMinFullDKAFromFullDKA() {
    std::unordered_map<long long, long long> oldNumConf;
    std::unordered_map<long long, long long> newNumConf;

    for (auto& conf: configurations_) {
        newNumConf.emplace(conf, acceptingConfigurations_.contains(conf));
    }

    while (!equalMaps(newNumConf, oldNumConf)) {
        oldNumConf = std::move(newNumConf);
        newNumConf.clear();

        std::unordered_map<long long, std::vector<long long>> transitions;
        for (auto& conf: configurations_) {
            transitions.emplace(conf, std::vector{oldNumConf[conf]});
        }

        for (auto& symbol: alphabet_) {
            for (auto& conf: configurations_) {
                long long insertedNum = oldNumConf[*transitions_[conf][std::string(&symbol, 1)].begin()];
                transitions[conf].push_back(insertedNum);
            }
        }

        for (auto& conf: configurations_) {
            for (auto& conf2: configurations_) {
                if (conf != conf2 && newNumConf.contains(conf2) && equalVectors(transitions[conf], transitions[conf2])) {
                    newNumConf.emplace(conf, newNumConf[conf2]);
                    break;
                }
            }

            if (!newNumConf.contains(conf)) {
                newNumConf.emplace(conf, conf);
            }
        }
    }

    std::set<long long> newConfigurations;
    for (auto& conf: configurations_) {
        newConfigurations.insert(newNumConf[conf]);
    }

    std::set<long long> newAcceptingConfigurations;
    for (auto& accept: acceptingConfigurations_) {
        newAcceptingConfigurations.insert(newNumConf[accept]);
    }

    NKA newNKA(newNumConf[q0_], alphabet_, newConfigurations, newAcceptingConfigurations);
    for (auto& pair: transitions_) {
        for (auto& transition: pair.second) {
            for (auto& final: transition.second) {
                newNKA.addTransition(newNumConf[pair.first], transition.first, newNumConf[final]);
            }
        }
    }

    *this = std::move(newNKA);
}
void NKA::makeMinFullDKA() {
    makeFullDKA();
    //createTexFileThisNKA("tests/DKA" + std::to_string(rand()) + ".tex", 5);
    makeMinFullDKAFromFullDKA();
    //createTexFileThisNKA("tests/MinDKA" + std::to_string(rand()) + ".tex", 5);
    delEmptyConfigurations();
}


bool NKA::operator==(const NKA& other) {
    NKA left = *this;
    NKA right = other;

    left.makeMinFullDKA();
    right.makeMinFullDKA();

    left.createTexFileThisNKA("tests/left.tex", 5);
    right.createTexFileThisNKA("tests/right.tex", 5);

    if (left.alphabet_.size() != right.alphabet_.size()) {
        return false;
    }

    if (left.configurations_.size() != right.configurations_.size()) {
        return false;
    }

    for (auto& symbol: left.alphabet_) {
        if (!right.alphabet_.contains(symbol)) {
            return false;
        }
    }

    std::unordered_map<long long, long long> leftToRightConfigurations;
    leftToRightConfigurations.emplace(left.q0_, right.q0_);

    std::queue<long long> queueConfigs;
    queueConfigs.push(left.q0_);

    while (!queueConfigs.empty()) {
        long long conf = queueConfigs.front();
        queueConfigs.pop();

        for (auto& pair: left.transitions_[conf]) {
            long long finishLeft = *pair.second.begin();
            long long finishRight = *right.transitions_[leftToRightConfigurations[conf]][pair.first].begin();
            if (leftToRightConfigurations.contains(finishLeft)) {
                if (leftToRightConfigurations[finishLeft] != finishRight) {
                    return false;
                }
            } else {
                leftToRightConfigurations.emplace(finishLeft, finishRight);
                queueConfigs.push(finishLeft);
                if (left.acceptingConfigurations_.contains(finishLeft) ^
                    right.acceptingConfigurations_.contains(finishRight)) {
                    return false;
                }
            }
        }
    }

    return true;
}
