//============================================================================
// Name        : HistogramGenerator.cpp
// Author      : Joshua Chu
// Version     :
// Copyright   : (C) JC 2014
// Description : Student score histogram generator
//============================================================================

#define DEBUG_MODE 0

#if DEBUG_MODE
#define debug(x) cout << x << endl
#else
#define debug(x) do {} while(0)
#endif

#include <iostream>

#include <cmath>
#include <map>
#include <string>
#include <algorithm>

#include <cassert>

#include "Simple_window.h"
#include "Graph.h"
using namespace std;
using namespace Graph_lib;

// number, grade of students; line, bar, axes, labels color; axes labels

int getInt() {
    string temp; int num = 0;
    while (true) {
        getline(cin, temp);

        try {
            num = stoi(temp);
        } catch (invalid_argument&) {
            cout << "  ~(#) Try again." << endl << "  ";
            continue;
        }
        break;
    }

    return num;
}
int getPositiveInt() {
    int num = 0;
    while(true) {
        num = getInt();
        if (num <= 0) {
            cout << "  ~(+) Try again." << endl << "  ";
            continue;
        }
        break;
    }

    return num;
}
int getLimitedInt(int lower, int upper) {
    int num = 0;
    while(true) {
        num = getInt();
        if (!(lower <= num && num <= upper)) {
            cout << "  ~([" + to_string(lower) + "," + to_string(upper) + "]) Try again." << endl << "  ";
            continue;
        }
        break;
    }

    return num;
}
vector<int> getStudentData() {
    bool test = false; string temp;
    cout << "Do you want to generate some test data? (y/n)" << endl;
    while (true) {
        getline(cin, temp);
        char c = tolower(temp[0]);

        if (c == 'y' || c == 'n') {
            test = c - 'n';
            break;
        }
        cout << "  ~(y/n) Try again." << endl << "  ";
    }

    vector<int> data;
    if (test) {
        srand(time(NULL) + rand() % 5000);
        for (int i = 0, len = rand() % 90 + 11; i < len; ++i)
            data.push_back(rand() % 100 + 1);
    }
    else {
        cout << "Number of students: ";
        int numStudents = getPositiveInt();

        for (int i = 0; i < numStudents; ++i) {
            cout << "Grade of student " << i + 1 << ": ";
            data.push_back(getLimitedInt(1, 100));
        }
    }

#if DEBUG_MODE
    for (int i = 0, len = data.size(); i < len; ++i) {
        cout << data[i] << ", ";
    }
    cout << endl;
#endif
    for (int i = 0, len = data.size(); i < len; ++i) {
        data[i] -= 1; // 1-10 becomes 0-9
        data[i] /= 10; // 0-9 becomes 0 :), 90-99 becomes 9
    }
#if DEBUG_MODE
    for (int i = 0, len = data.size(); i < len; ++i) {
        cout << data[i] << ", ";
    }
    cout << endl;
#endif
    vector<int> partitioned;
    int num = 0;
    for (int i = 0; i <= 9; ++i) {
        num = count(data.begin(), data.end(), i);
        partitioned.push_back(num);
    }
#if DEBUG_MODE
    assert(partitioned.size() == 10);
    for (int i = 0; i <= 9; ++i) {
        cout << partitioned[i] << ", ";
    }
#endif
    return partitioned;
}

struct Style {
    Color lineColor;
    Color fillColor;
    Line_style lineStyle;

    Style(Color lc, Color fc, Line_style ls) : lineColor(lc), fillColor(fc), lineStyle(ls) {}
    Style() : lineColor(Color::invisible), fillColor(Color::invisible), lineStyle(-1) {}
};
string getMatchingString(string matches[], int matches_len) {
    bool matching = false; string temp;
    while(true) {
        getline(cin, temp);
        for (int i = 0; i < matches_len; ++i) {
            matching = (temp == matches[i]);
            if (matching)
                break;
        }
        if (matching)
            break;
        cout << "  ~(valid) Try again." << endl << "  ";
    }

    return temp;
}
Color getLineColor() {
    string matches[] = { "red", "blue", "green", "yellow", "white", "black" };
    int matches_len = 6;
    string matched = getMatchingString(matches, matches_len);

    Color colorArr[] = {Color::red, Color::blue, Color::green, Color::yellow, Color::white, Color::black };
    for (int i = 0; i < matches_len; ++i) {
        if (matched == matches[i])
            return colorArr[i];
    }
    return Color::black;
}
Color getFillColor() {
    return getLineColor();
}
Line_style getLineStyle() {
    string matches[] = { "solid", "dash", "dot", "dashdot", "dashdotdot" };
    int matches_len = 5;
    string matched = getMatchingString(matches, matches_len);

    Line_style lsArr[] = { Line_style::solid, Line_style::dash, Line_style::dot, Line_style::dashdot, Line_style::dashdotdot };
    for (int i = 0; i < matches_len; ++i) {
        if (matched == matches[i])
            return lsArr[i];
    }
    return Line_style::solid;
}
Line_style getLineWeight(Line_style ls) {
    Line_style::Line_style_type lst = (Line_style::Line_style_type)ls.style();
    int weight = getPositiveInt();
    return Line_style(lst, weight);
}
vector<Style> getStyleProperties() {
    bool custom = false; string temp;
    cout << "Would you like custom formatting of your histogram? (y/n)" << endl;
    while (true) {
        getline(cin, temp);
        char c = tolower(temp[0]);

        if (c == 'y' || c == 'n') {
            custom = c - 'n';
            break;
        }
        cout << "  ~(y/n) Try again." << endl;
    }

    vector<Style> properties(8);
    if (!custom) {
        //                    Line color    Fill color        Line style
        properties[0] = Style(Color::black, Color::invisible, Line_style::solid               ); // x-axis
        properties[1] = Style(Color::black, Color::invisible, Line_style::solid               ); // y-axis
        properties[2] = Style(Color::blue,  Color::invisible, -1                              ); // x-axis label
        properties[3] = Style(Color::black, Color::invisible, -1                              ); // x-data labels
        properties[4] = Style(Color::blue,  Color::invisible, -1                              ); // y-axis label
        properties[5] = Style(Color::black, Color::invisible, -1                              ); // y-data labels
        properties[6] = Style(Color::blue,  Color::invisible, Line_style(Line_style::dash, 3) ); // line
        properties[7] = Style(Color::black, Color::red,       Line_style::solid               ); // bar
        return properties;
    }

    string lcComponentNames[] = { "x-axis", "y-axis", "x-axis label", "x-data labels", "y-axis label", "y-data labels", "line", "bar"};
    string lsComponentNames[] = { "x-axis", "y-axis", ""            , ""             , ""            , ""             , "line", "bar"};
    string lwComponentNames[] = { "x-axis", "y-axis", ""            , ""             , ""            , ""             , "line", "bar"};
    string fcComponentNames[] = { ""      , ""      , ""            , ""             , ""            , ""             , ""    , "bar"};
    for (int i = 0; i < 8; ++i) {
        if (lcComponentNames[i] != "") {
            cout << "Line color of " << lcComponentNames[i] << ": ";
            properties[i].lineColor = getLineColor();
        }
        if (lsComponentNames[i] != "") {
            cout << "Line style of " << lsComponentNames[i] << ": ";
            properties[i].lineStyle = getLineStyle();
        }
        if (lwComponentNames[i] != "") {
            cout << "Line weight of " << lwComponentNames[i] << ": ";
            properties[i].lineStyle = getLineWeight(properties[i].lineStyle);
        }
        if (fcComponentNames[i] != "") {
            cout << "Fill color of " << fcComponentNames[i] << ": ";
            properties[i].fillColor = getFillColor();
        }
        cout << endl;
    }
    return properties;
}

int main() {
    cout << "Histogram Generator 2000" << endl;
    cout << "This generator does NOT ACCEPT 0 for a grade! Use 1 instead." << endl;
    cout << "------------------------" << endl << endl;

    // partitioned grades i.e., partGrades[0] contains # of grades from [1, 10)
    // psst! 0 to 100 is 101 distinct values! I go from 1 to 100
    vector<int> partGrades = getStudentData();
    /* will hold the line color, fill color, line-style, and line-weight of
     *  - x-axis            l   0   s   w
     *  - y-axis            l   0   s   w
     *  - x-axis label      l   0   0   0
     *  - x-data labels     l   0   0   0
     *  - y-axis label      l   0   0   0
     *  - y-data labels     l   0   0   0
     *  - line              l   0   s   w
     *  - bar               l   f   s   w
     * in that order.
     *
     * l: this component can have a custom line color
     * f: this component can have a custom fill color
     * s: this component can have a custom line style
     * w: this component can have a custom line weight
     */
    vector<Style> styleVec = getStyleProperties();

    // set chart area size
    const int H = 600, W = 600;
    // set padding around the chart area
    const int PAD = 50;

    // determine the size of the y-axis unit based on the maximum value in grades
    int max = partGrades[0];
    for (int i = 0, len = partGrades.size(); i < len; ++i)
        if (partGrades[i] > max)
            max = partGrades[i];
    const double Y_UNIT_SIZE =  max / 10.0;
    const int PX_PER_UNIT = H / 10.0;

    // create window, axes with labels, bars, and open polyline
    Simple_window win(Point(50,50), W + 2*PAD, H + 2*PAD, "Histogram Generator");

    Axis xAxis(Axis::x, Point(PAD, PAD + H), W, 10, "");
    Axis yAxis(Axis::y, Point(PAD, PAD + H), H, max, "");

    Text xAxisLabel(Point(PAD + W + 5, PAD + H + 20), "Grades");
    Vector_ref<Text> xDataLabels;
    for (int i = 0; i < 10; ++i) {
        //  0       1       2       3       4       5       6       7       8       9
        //  1-10    11-20   21-30   31-40   41-50   51-60   61-70   71-80   81-90   91-100
        Point p(PAD + (PX_PER_UNIT/2) - 15 + i*PX_PER_UNIT, PAD + H + 15);
        string s( (i > 0 ? string(1, i + '0') : string(1, ' '))
                  + "1"
                  + "-"
                  + (i < 9 ? string(1, i + '1') : "1")
                  + "0"
                  + (i == 9 ? "0" : " ") );
        xDataLabels.push_back(new Text(p, s));
    }

    Text yAxisLabel(Point(3, PAD - 7), "Number of Students");
    Text yDataLabel(Point(PAD - 30, PAD + 5), "" + to_string(max)); // to_string conveniently converts integer to string (in <string>)

    Open_polyline line;
    for (int i = 0, len = partGrades.size(); i < len; ++i) {
        line.add(Point(PAD + (PX_PER_UNIT/2) + i*PX_PER_UNIT, PAD+H - (partGrades[i] / Y_UNIT_SIZE)*PX_PER_UNIT));
    }

    Vector_ref<Rectangle> bars;
    for (int i = 0, len = partGrades.size(); i < len; ++i) {
        int height = (partGrades[i]/Y_UNIT_SIZE)*PX_PER_UNIT;
        bars.push_back(new Rectangle( Point(PAD +   (PX_PER_UNIT/3) + i*PX_PER_UNIT, (height > 0 ? PAD+H - height : PAD+H - 1)),
                                      Point(PAD + 2*(PX_PER_UNIT/3) + i*PX_PER_UNIT, PAD+H) ));
    }

    // set line color data
    xAxis.set_color(styleVec[0].lineColor);
    yAxis.set_color(styleVec[1].lineColor);

    xAxisLabel.set_color(styleVec[2].lineColor);
    for (int i = 0, len = xDataLabels.size(); i < len; ++i)
        xDataLabels[i].set_color(styleVec[3].lineColor);
    yAxisLabel.set_color(styleVec[4].lineColor);
    yDataLabel.set_color(styleVec[5].lineColor);

    line.set_color(styleVec[6].lineColor);
    for (int i = 0, len = bars.size(); i < len; ++i) {
        bars[i].set_color(styleVec[7].lineColor);
    }

    // set fill color data
    for (int i = 0, len = bars.size(); i < len; ++i) {
        bars[i].set_fill_color(styleVec[7].fillColor);
    }

    // set line style data
    xAxis.set_style(styleVec[0].lineStyle);
    yAxis.set_style(styleVec[1].lineStyle);
    line.set_style(styleVec[6].lineStyle);
    for (int i = 0, len = bars.size(); i < len; ++i) {
        bars[i].set_style(styleVec[7].lineStyle);
    }

    // show window
    win.attach(xAxis);
    win.attach(yAxis);

    win.attach(xAxisLabel);
    for (int i = 0, len = xDataLabels.size(); i < len; ++i)
        win.attach(xDataLabels[i]);
    win.attach(yAxisLabel);
    win.attach(yDataLabel);

    for (int i = 0, len = bars.size(); i < len; ++i)
        win.attach(bars[i]);
    win.attach(line);

    win.wait_for_button();

	return 0;
}
