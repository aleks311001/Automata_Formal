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

const std::string EPS = ""; ///< Epsilon word


/**
 * @brief NKA class
 * @details This class can make DKA, full DKA, anti DKA, regular expression and draw in file automata.
 */
class NKA {
private:
    using TransitionsType = std::unordered_map<long long, std::unordered_map<std::string, std::set<long long>>>;

    std::set<long long> configurations_;
    std::set<char> alphabet_;
    TransitionsType transitions_;
    long long q0_;
    std::set<long long> acceptingConfigurations_;


    void addTransitionWithoutCheck_(long long left, const std::string& word, long long right);
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

    /*bool hasWayToAccepting_(long long start,
                            std::unordered_map<long long, bool>& newConfigurations,
                            std::unordered_map<long long, bool>& wasIn);*/

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

    /**
     * Write prefix (includes, begins...) in file
     * @param [in, out] file
     */
    void writePrefix_(std::ofstream& file);
    /**
     * Write alphabet in file
     * @param [in, out] file file stream to write
     */
    void writeAlphabet_(std::ofstream& file);
    /**
     * Write accepting configurations in file
     * @param [in, out] file file stream to write
     * @param [in] numsConfigurations map with configuration -> its number
     */
    void writeAcceptingConfigurations_(std::ofstream& file,
                                      const std::unordered_map<long long, size_t>& numsConfigurations);

    /**
     * Draw circle of configurations in file
     * @param [in, out] file file stream to draw
     * @param [in] numsConfigurations map with configuration -> its number
     * @param [in] r circle radius
     */
    void writeConfigurations_(std::ofstream& file,
                              const std::unordered_map<long long, size_t>& numsConfigurations,
                              double r);
    /**
     * Draw edge in file
     * @param [in, out] file file stream to draw
     * @param [in] word symbols on edge
     * @param [in] startConf start configuration of edge
     * @param [in] finishConf finish configuration of edge
     * @param [in] daddies map configuration -> all configurations which are daddy our configuration
     * @param [in] numsConfigurations map with configuration -> its number
     */
    void writeEdge_(std::ofstream& file, std::string word,
                    long long startConf, long long finishConf,/* int angle,*/
                    const std::unordered_map<long long, std::set<long long>>& daddies,
                    const std::unordered_map<long long, size_t>& numsConfigurations);
    /**
     * Make map that contain for each configuration set of its daddies
     * @return made map
     */
    std::unordered_map<long long, std::set<long long>> findDaddies();
    /**
     * Draw all edges in file
     * @param [in, out] file file stream to draw
     * @param [in] numsConfigurations
     */
    void writeTransitions_(std::ofstream& file,
                           std::unordered_map<long long, size_t>& numsConfigurations);
    void writeMainPart_(std::ofstream& file, double r);
    void writeSuffix_(std::ofstream& file, bool writeRegular);

    /// Create new configurations and edges from old accepting configurations to this
    void makeOneAcceptingConfiguration_();
    /// Replace EPS to "1"
    void replaceEpsilonToOne_();
    /**
     * For all edges \f$ w_1,\ldots, w_k \f$ from \f$ q_1 \f$ to \f$ q_2 \f$
     * make one edge \f$ w_1+w_2+\ldots+w_k \f$ from \f$ q_1 \f$ to \f$ q_2 \f$ and delete old edges
     * @param symbol that stand between two words, as '+' or ','
     */
    void makeOneEdgeForAllPairs_(char symbolBetweenWords = '+', const std::string& replaceEPS = "1");
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

    /**
     * Do last step of making regular, if left one configuration
     * @param [in] conf left configuration
     * @return final regular
     */
    std::string getRegularIfOneConfiguration_(long long conf);
    /**
     * Do last step of making regular, if left two configuration: start and accepting
     * @param [in] start start configuration
     * @param [in] accept accepting configuration
     * @return final regular
     */
    std::string getRegularIfTwoConfiguration_(long long start, long long accept);

public:
    NKA(long long q0 = 0,
        const std::set<char>& alphabet = std::set<char>(),
        const std::set<long long>& configurations = std::set<long long>(),
        const std::set<long long>& acceptingConfigurations = std::set<long long>(),
        const TransitionsType& transitions = TransitionsType());
    NKA(long long q0,
        const std::set<char>& alphabet,
        long long numConfigurations,
        const std::set<long long>& acceptingConfigurations = std::set<long long>(),
        const TransitionsType& transitions = TransitionsType());

    NKA(const NKA& other);
    NKA(NKA&& other);
    NKA& operator= (const NKA& other);
    NKA& operator= (NKA&& other);

    /**
     * Add configuration "add"
     * @param [in] add added configuration
     * @throw invalid_argument if added configuration already contains
     */
    void addConfiguration(long long add);
    /**
     * Add a symbol in alphabet
     * @param [in] add added symbol
     * @throw invalid_argument if added symbol already contains
     */
    void addSymbol(char add);
    /**
     * Add edge from left to right with value word
     * @param [in] left start of edge configuration
     * @param [in] word value on edge
     * @param [in] right finish of edge configuration
     * @throw invalid_argument if added configuration or word does not contains in configurations or alphabet
     */
    void addTransition(long long left, const std::string& word, long long right);
    /**
     * Make configuration "add" accepting
     * @param add made configuration
     * @throw invalid_argument if added configuration already contains in accepting configuration
     */
    void addAcceptingConfiguration(long long add);
    /**
     * Delete configuration "add" from accepting
     * @param del deleted configuration
     * @throw invalid_argument if deleted configuration does not contains in accepting configuration
     */
    void delAcceptingConfiguration(long long del);
    void delConfiguration(long long conf);

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
     * Transform this in NKA with a single edge type from each state
     */
    void makeExplicitWays();

    /**
     * Make DKA. Call:
     * @code
     *     replaceMultiSymbolsEdges();
     *     changeEpsTransitions();
     *     makeExplicitWays();
     * @endcode
     */
    void makeDKA();

    /// Make full DKA from DKA
    void makeFullDKAFromDKA();
    /// Make full DKA from NKA
    void makeFullDKA();
    ///Make anti DKA from full DKA
    void makeAntiDKAFromFullDKA();
    ///Make anti DKA from NKA
    void makeAntiDKA();
    ///Make minimal full DKA from full DKA
    void makeMinFullDKAFromFullDKA();
    ///Make minimal full DKA from NKA
    void makeMinFullDKA();

    /**
     * @brief Create file with this NKA.
     * @details Write alphabet, q0, accepting configurations and draw NKA
     * @param [in] filename name of created file
     * @param [in] r radius of configurations circle
     * @param [in] writeRegular if true make and write in file regular expression
     */
    void createTexFileThisNKA(const std::string& filename, double r, bool writeRegular = true);
    /**
     * @brief Create file with this NKA without copy NKA, but break this NKA.
     * @details Write alphabet, q0, accepting configurations and draw NKA
     * @param [in] filename name of created file
     * @param [in] r radius of configurations circle
     */
    void createTexFileThisNKAWithoutCopy(const std::string& filename, double r = 5);

    /**
     * add regular symbols {+, ^, *, (, ), 1} in alphabet
     */
    void addRegularSymbols();

    /**
     * Make regular this NKA
     * @return regular
     */
    std::string makeRegular();
    /**
     * Make regular this NKA without copy, but broke it
     * @return regular
     */
    std::string makeRegularWithoutCopy();

    bool operator==(const NKA& other);
};

#endif //FORMAL__NKA_H_
