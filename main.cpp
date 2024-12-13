#include "Header.h"

int main() {
    string class_name;
    int num = 0;
    int index = 0;
    double current_totgrade = 0.0; // current grade
    double current_totweight = 0.0; // current weight
    double total_possible_grade = 0.0; // max grade
    double total_possible_weight = 0.0; // weight with all grades in
    double cur_per = 0.0;
    double max_per = 0.0;
    double min_per = 0.0;
    double num_of_sec = 0.0; // number of sections
    string y_n; // yes or no
    int num_graded = 0;
    double min_grade = 0.0;

    
    ifstream file("assignments.txt");

    if (!file) {
        cerr << "Error: Could not open file." << endl;
        return 1;
    }

    file >> num;
    vector<assignment> assignments(num);
    file >> num_of_sec;
    vector<double> sec_weight(num_of_sec);
    vector<double> num_in_sec(num_of_sec);
    for (int j = 0; j < num_of_sec; j++) {
        file >> num_in_sec[j];
        file >> sec_weight[j];
        file >> num_graded;

        for (int i = 0; i < num_in_sec[j]; i++) {
            file >> assignments[index].name;
            file >> y_n;

            if (y_n == "y") {
                assignments[index].graded = true;
                file >> assignments[index].norm_grade;// Read the grade for graded assignments
                file >> assignments[index].norm_weight;
                assignments[index].weight = sec_weight[j] / num_graded;
                assignments[index].grade = (assignments[index].norm_grade / assignments[index].norm_weight) * assignments[index].weight;
            }
            else {
                assignments[index].graded = false;
                assignments[index].grade = 0;  // No grade for ungraded assignments
                assignments[index].weight = sec_weight[j] / num_in_sec[j];
            }
            index++;
        }

    }

    file.close();  // Close the file
    index = 0;
    // Calculating current and maximum grades
    for (int j = 0; j < num_of_sec; j++) {
        for (int i = 0; i < num_in_sec[j]; i++) {
            if (assignments[index].graded) {
                current_totgrade += assignments[index].grade;
                current_totweight += assignments[index].weight;
                total_possible_grade += (assignments[index].grade / assignments[index].weight) * (sec_weight[j] / num_in_sec[j]);
                total_possible_weight += sec_weight[j]/num_in_sec[j];
                min_grade += (assignments[index].norm_grade / assignments[index].norm_weight) * (sec_weight[j] / num_in_sec[j]);
            }
            else {
                total_possible_grade += assignments[index].weight;
                total_possible_weight += assignments[index].weight;
            }
            index++;
        }
    }


    cur_per = (current_totgrade / current_totweight) * 100;
    max_per = (total_possible_grade / total_possible_weight) * 100;
    min_per = (min_grade / total_possible_weight) * 100;

    index = 0;
    // Output with fixed precision
    cout << fixed << setprecision(2);  // Output with 2 decimal places
    cout << "Current Grade: " << current_totgrade << "/" << current_totweight << " = " << cur_per << "%" << endl;
    cout << "Max Grade: " << total_possible_grade << "/" << total_possible_weight << " = " << max_per << "%" << endl;
    cout << "Min Grade: " << min_grade << "/" << total_possible_weight << " = " << min_per << "%" << endl << endl;


    double desired_grade = 0.0;
    vector<assignment> ungraded;
    vector<assignment> ordered;
    assignment least;
    least.weight = 9999999.0;
    int stopped = 0;
    double compare = 0.0;
    compare += min_grade;


    cout << "Would you like to see what you need on remaining assignments to get a desired grade(y/n): ";
    cin >> y_n;
    cout << endl;
    if (y_n == "y") {
        cout << "Enter desired grade as percent: ";
        cin >> desired_grade;
        cout << endl;
        if (desired_grade > max_per) {
            cout << "Sorry thats not possible, better luck next time :(" << endl;
        }
        else if (desired_grade < min_per) {
            cout << "Sorry thats not possible, youre better than that" << endl;
        }
        else {
            desired_grade = ((desired_grade / 100) * total_possible_weight);
            for (int i = 0; i < assignments.size(); i++) {
                if (!assignments[i].graded) {
                    ungraded.push_back(assignments[i]);
                }
            }
            for (int j = 0; j < ungraded.size(); j++) {
                for (int i = 0; i < ungraded.size(); i++) {
                    if (ungraded[i].weight < least.weight && !ungraded[i].taken) {
                        least = ungraded[i];
                        least.index_least = i;
                    }
                }
                ungraded[least.index_least].taken = true;
                ordered.push_back(least);
                least.weight = 9999999.0;
                

            }
            for (int i = 0; i < ordered.size(); i++) {
                compare += ordered[i].weight;
                if (compare > desired_grade) {
                    stopped = i;
                    compare -= ordered[i].weight;
                    break;
                }
            }
            for (int i = 0; i < stopped; i++) {
                cout << ordered[i].name << " needs " << ordered[i].weight << "/" << ordered[i].weight << endl;
            }
            cout << ordered[stopped].name << " needs " << desired_grade - compare << "/" << ordered[stopped].weight << endl;
            if (stopped < ordered.size() - 1) {
                for (int i = stopped + 1; i < ordered.size(); i++) {
                    cout << ordered[i].name << " needs " << "0" << "/" << ordered[i].weight << endl;
                }
            }
        }
    }

    return 0;
}
