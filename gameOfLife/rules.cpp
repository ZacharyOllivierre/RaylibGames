#include "rules.h"

Rules::Rules() : displayedRule(0), lastClickedSection(NEIGHBORHOOD_NUM)
{
    changed = true;
}

Rules::~Rules() {}

void Rules::addRule(float min, float max, int output, int neighAffiliation)
{
    cellRules.push_back({min, max, output, neighAffiliation});
}

void Rules::insertRuleAtDisplay(float min, float max, int output, int neighAffiliation)
{
    cellRules.insert(cellRules.begin() + displayedRule, {min, max, output, neighAffiliation});

    clampDisplayedRule();
}

void Rules::insertRuleAfterDisplay(float min, float max, int output, int neighAffiliation)
{
    cellRules.insert(cellRules.begin() + displayedRule + 1, {min, max, output, neighAffiliation});

    displayedRule++;

    clampDisplayedRule();
}

void Rules::removeDisplayRule()
{
    if (cellRules.size() > 1)
    {
        cellRules.erase(cellRules.begin() + displayedRule);

        clampDisplayedRule();
    }
}

void Rules::incDisplayedRules()
{
    displayedRule++;
    clampDisplayedRule();
}
void Rules::decDisplayedRules()
{
    displayedRule--;
    clampDisplayedRule();
}

void Rules::adjustLastClicked(int delta)
{
    // Early out if its empty
    if (cellRules.empty() || displayedRule < 0 || displayedRule >= (int)cellRules.size())
        return;

    switch (lastClickedSection)
    {
    case NEIGHBORHOOD_NUM:
        cellRules[displayedRule].neighAffiliation += delta;
        wholeBoundsCheck(0, 1, cellRules[displayedRule].neighAffiliation);
        break;

    case MIN_PERCENT:
        cellRules[displayedRule].min += delta / 100.0;
        percentBoundsCheck(0.00, 1.00, cellRules[displayedRule].min);
        break;

    case MAX_PERCENT:
        cellRules[displayedRule].max += delta / 100.0;
        percentBoundsCheck(0.00, 1.00, cellRules[displayedRule].max);
        break;

    case OUTPUT:
        cellRules[displayedRule].output += delta;
        wholeBoundsCheck(0, 1, cellRules[displayedRule].output);
        break;

    default:
        break;
    }

    changed = true;
}

// Getters
CellRule *Rules::getRuleIndex(int index)
{
    // early out if index is out of bounds
    if (index < 0 || index >= (int)cellRules.size())
        return nullptr;

    return &cellRules[index];
}

void Rules::getDisplayedRule(CellRule &rule, int &ruleNum)
{

    if (!cellRules.empty())
    {
        rule = cellRules[displayedRule];
    }
    else
    {
        rule = {0, 0, 0, 0};
    }
    ruleNum = displayedRule;
}

// Setters
void Rules::setLastClickedSection(RuleSections lastClicked)
{
    if (lastClicked != RuleSections::RULE_NUM)
    {
        lastClickedSection = lastClicked;
    }
}

void Rules::setChanged(bool changed) { this->changed = changed; }

// Private
void Rules::clampDisplayedRule()
{
    if (cellRules.empty())
    {
        displayedRule = 0;
    }
    else if (displayedRule < 0)
    {
        displayedRule = 0;
    }
    else if (displayedRule >= (int)cellRules.size())
    {
        displayedRule = (int)cellRules.size() - 1;
    }

    changed = true;
}

void Rules::percentBoundsCheck(float min, float max, float &val)
{
    // if val is below minimum set to min
    if (val <= min)
    {
        val = min;
    }
    else if (val >= max)
    {
        val = max;
    }
}

void Rules::wholeBoundsCheck(int min, int max, int &val)
{
    // if val is below minimum set to min
    if (val <= min)
    {
        val = min;
    }
    else if (val >= max)
    {
        val = max;
    }
}