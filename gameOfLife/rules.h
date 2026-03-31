#pragma once
#include "types.h"
#include <vector>
#include <raylib.h>

class Rules
{

private:
    RuleSections lastClickedSection;

    int displayedRule;
    std::vector<CellRule> cellRules;

    bool changed;

    void clampDisplayedRule();

    void percentBoundsCheck(float min, float max, float &val);
    void wholeBoundsCheck(int min, int max, int &val);

public:
    Rules();
    ~Rules();

    void addRule(float min, float max, int output, int neighAffiliation);
    void insertRuleAtDisplay(float min = 0, float max = 0, int output = 0, int neighAffiliation = 0);
    void insertRuleAfterDisplay(float min = 0, float max = 0, int output = 0, int neighAffiliation = 0);
    void removeDisplayRule();

    void incDisplayedRules();
    void decDisplayedRules();

    void adjustLastClicked(int delta);

    // Getters
    CellRule *getRuleIndex(int index);
    void getDisplayedRule(CellRule &rule, int &ruleNum);
    inline int getDisplayedRuleNum() { return displayedRule; }
    inline std::vector<CellRule> *getCellRulesPtr() { return &cellRules; }
    inline int getCellRulesLength() { return (int)cellRules.size(); }
    inline bool getChanged() { return changed; }

    // Setters
    void setLastClickedSection(RuleSections lastClicked);
    void setChanged(bool changed);
};