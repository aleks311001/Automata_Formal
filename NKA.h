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
     * @param [in] configurations subset all configurations
     * @param [in] indexesConfigs map: configuration -> number
     * @return new configuration
     */
    static long long makeConfigurationFromOthers_(const std::set<long long>& configurations,
                                                  const std::unordered_map<long long, size_t>& indexesConfigs);
    /**
     * Check that set of configurations has accepting configuration
     * @param [in] configurations checked configurations
     * @return result of check
     */
    bool checkSetConfigsOnAccepting_(const std::set<long long>& configurations);

    void writeConfigurations_(std::ofstream& file,
                              std::unordered_map<long long, size_t>& numsConfigurations,
                              int r);
    void writeEdge_(std::ofstream& file, std::string word,
                    long long startConf, long long finishConf,
                    std::unordered_map<long long, std::set<long long>>& daddies,
                    std::unordered_map<long long, size_t>& numsConfigurations);
    std::unordered_map<long long, std::set<long long>> findDaddies();
    void writeTransitions_(std::ofstream& file,
                           std::unordered_map<long long, size_t>& numsConfigurations);

    /// Create new configurations and edges from old accepting configurations to this
    void makeOneAcceptingConfiguration_();
    /// Replace EPS to "1"
    void replaceEpsilonToOne_();
    /// For all edges w1,..., w_k from q1 to q2 make one edge w1+w2+...+w_k from q1 to q2 and delete old edges
    void makeOneEdgeForAllPairs_();
    /**
     * Add brackets to string, if len of string > 1
     * @param [in] string
     */
    static std::string getBracketedExpression_(const std::string& string);
    /**
     * Add brackets and star to string
     * @param [in] string
     */
    static std::string getStarsExpression_(const  std::string& string);

    /**
     * Union left, loop, right in \f$ left (loop)^* right \f$
     * @param [in] left, loop, right text on edges
     * @return result union edge
     */
    static std::string unionEdgeLoopEdge(const std::string& left, const std::string& loop, const std::string& right);
    /**
     * Replace all paths \f$ q_1 \xrightarrow{edge_{left}} conf \xrightarrow{loop} conf \xrightarrow{edge_{right}} q_2 \f$
     * on edge \f$ q_1 \xrightarrow{edge_{left} (loop)^{*} edge_{right}} q_2 \f$
     * @param [in] conf skipping configuration
     */
    void skipConfiguration_(long long conf);

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

    /*void delConfiguration(long long del);
    void delSymbol(char del);
    void delTransition(long long left, const std::string& word, long long right);
    void delAcceptingConfiguration(long long del);*/

    /**
     * Generate and add unique configuration
     * @param [in] min generated configuration greater then this
     * @return added configuration
     */
    long long addNewConfiguration(long long min = 0);

    /// Replace multi symbols edge on single-symbol edges
    void replaceMultiSymbolsEdges();

    /// Change all epsilon transitions on one-letter.
    void changeEpsTransitions();

    /// Delete All configurations which has not edges
    void delEmptyConfigurations();

    /**
     * transform this in NKA with a single edge type from each state
     */
    void makeExplicitWays();
    void makeDKA();

    void makeFullDKAFromDKA();
    void makeAntiDKAFromFullDKA();

    void createTexFileThisNKA(const std::string& filename, double r, bool writeRegular = true);

    void addRegularSymbols() {
        alphabet_.insert('+');
        alphabet_.insert('*');
        alphabet_.insert('(');
        alphabet_.insert(')');
        alphabet_.insert('1');
        alphabet_.insert('^');
    }

    std::string makeRegular();
};


#endif //FORMAL__NKA_H_
