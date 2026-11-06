#include "DescriptionFormatter.h"

#include <iostream>
#include <string_view>

int main() {
    using PoisonDescriptions::DescriptionFormatter;
    int failures = 0;
    const auto expect = [&failures](std::string_view a_case, std::string_view a_existing, std::string_view a_poison, std::string_view a_expected) {
        const auto actual = DescriptionFormatter::AppendPoison(a_existing, a_poison);
        if (actual != a_expected) {
            std::cerr << a_case << ": expected " << a_expected << ", got " << actual << '\n';
            ++failures;
        }
    };

    expect("unenchanted poisoned weapon", "", "Causes 15 points of poison damage.", "<font color='#4EDB43'>Causes 15 points of poison damage.</font>");
    expect("enchanted poisoned weapon", "Burns the target for 10 points.", "Causes 15 points of poison damage.",
           "Burns the target for 10 points.<br><font color='#4EDB43'>Causes 15 points of poison damage.</font>");
    expect("existing markup retains its color", "<font color='#FFFFFF'>Fire damage.</font>", "Paralysis for 5 seconds.",
           "<font color='#FFFFFF'>Fire damage.</font><br><font color='#4EDB43'>Paralysis for 5 seconds.</font>");
    expect("multiple effects stay green", "", "Damage health. Slow for 10 seconds.", "<font color='#4EDB43'>Damage health. Slow for 10 seconds.</font>");
    expect("effect text cannot override green", "", "Venom <font color='#FFFFFF'> & damage", "<font color='#4EDB43'>Venom &lt;font color='#FFFFFF'&gt; &amp; damage</font>");
    expect("effect line breaks", "", "Damage.\r\nSlow.", "<font color='#4EDB43'>Damage.<br>Slow.</font>");
    const auto highlighted = [](std::string_view a_value) {
        return std::string{DescriptionFormatter::kHighlightBegin} + std::string{a_value} + std::string{DescriptionFormatter::kHighlightEnd};
    };
    expect("native values use softer green and body color resumes", "", "Causes " + highlighted("15") + " damage for " + highlighted("10") + " seconds.",
           "<font color='#4EDB43'>Causes </font><font color='#A3CFA0'>15</font><font color='#4EDB43'> damage for </font><font color='#A3CFA0'>10</font><font color='#4EDB43'> seconds.</font>");
    expect("existing description stays unchanged with poison highlights", "<font color='#FFFFFF'>Fire damage.</font>", "Paralysis for " + highlighted("5") + " seconds.",
           "<font color='#FFFFFF'>Fire damage.</font><br><font color='#4EDB43'>Paralysis for </font><font color='#A3CFA0'>5</font><font color='#4EDB43'> seconds.</font>");
    expect("highlighted text remains escaped", "", highlighted("Health < 20 & stamina"),
           "<font color='#4EDB43'></font><font color='#A3CFA0'>Health &lt; 20 &amp; stamina</font><font color='#4EDB43'></font>");
    expect("unpoisoned weapon", "Fire damage.", "", "Fire damage.");
    expect("empty weapon", "", "", "");
    return failures ? 1 : 0;
}
