//
// Created by aleks311001 on 22.09.2020.
//

#ifndef FORMAL__NKA_H_
#define FORMAL__NKA_H_


#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include <queue>
#include <exception>
#include <limits>
#include <fstream>
#include <cmath>

const std::string EPS = "";

class NKA {
private:
    using TransitionsType = std::unordered_map<long long, std::unordered_map<std::string, std::set<long long>>>;

    std::set<long long> configurations_;
    std::set<char> alphabet_;
    TransitionsType transitions_;
    long long q0_;
    std::set<long long> acceptingConfigurations_;

    /**
     * @details Find all ends configurations of ways from start, that has epsilon transitions,
     * but last transition is not epsilon. start-EPS->q_1-EPS->q_2-EPS->....-EPS->q_n-a->q in endWayConfigurations
     * @param [in] start starting configuration
     * @param [in, out] wasIn set of configurations, that has way from start
     * @param [out] endWayConfigurations all ends configurations of ways with
     * @param [out] hasAcceptingConf true if exist epsilon way from start to accepting configuration
     */
    void findAllWaysOnEpsEdges_(long long start,
                               std::set<long long>& wasIn,
                               std::list<std::pair<std::string, long long>>& endWayConfigurations,
                               bool& hasAcceptingConf);
    /**
     * Add all edges, that can replace epsilon edges for ways from start.
     * If exist way: start-EPS->q_1-EPS->q_2-EPS->....-EPS->q_n-a->q we add start-a->q
     * Also that function make configurations accepting, if exist epsilon way from it to accepting configuration.
     * @param [in] start starting configuration
     * @param [in, out] wasIn set of configurations, that has way from start
     */
    void addTransitionsInEpsWays_(long long start, std::set<long long>& wasIn);
    /**
     * Remove all epsilon transitions
     */
    void removeEpsilonTransitions_();

    /**
     * For each configuration make a unique number in [0, number of Configurations)
     * @return map: configuration -> number
     */
    std::unordered_map<long long, size_t> getMapConfigurationToNumber_();
    /**
     * Make big configuration for new NKA, as a subset of set all configurations
     * @param configurations subset all configurations
     * @param indexesConfigs map: configuration -> number
     * @return new configuration
     */
    static long long makeConfigurationFromOthers_(const std::set<long long>& configurations,
                                                  const std::unordered_map<long long, size_t>& indexesConfigs);
    /**
     * Check that set of configurations has accepting configuration
     * @param configurations checked configurations
     * @return result of check
     */
    bool checkSetConfigsOnAccepting_(const std::set<long long>& configurations);

    void writeConfigurations_(std::ofstream& file,
                              std::unordered_map<long long, size_t>& numsConfigurations,
                              int r);
    void writeTransitions_(std::ofstream& file,
                           std::unordered_map<long long, size_t>& numsConfigurations);

public:
    NKA(long long q0 = 0,
        const std::set<char>& alphabet = std::set<char>(),
        const std::set<long long>& configurations = std::set<long long>(),
        const std::set<long long>& acceptingConfigurations_ = std::set<long long>(),
        const TransitionsType& transitions = TransitionsType());
    NKA(const NKA& other);
    NKA(NKA&& other);
    NKA& operator= (const NKA& other);
    NKA& operator= (NKA&& other);

    void changeQ0(long long q0);

    void addConfiguration(long long add);
    void addSymbol(char add);
    void addTransition(long long left, const std::string& word, long long right);
    void addAcceptingConfiguration(long long add);

    void delConfiguration(long long del);
    void delSymbol(char del);
    void delTransition(long long left, const std::string& word, long long right);
    void delAcceptingConfiguration(long long del);

    /// Change all epsilon transitions on one-letter.
    void changeEpsTransitions();

    /// Delete All configurations which has not edges
    void delEmptyConfigurations();

    /**
     * transform this in NKA with a single edge type from each state
     */
    void makeExplicitWays();
    void makeDKA();

    void createTexFileThisNKA(const std::string& filename, int r);
};


#endif //FORMAL__NKA_H_
