#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

void writeDataToFile() {
    TFile* file = TFile::Open("/home/lecamvi/Documents/TOF_thesis/output/run2134_outputTree.root", "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    TTree* tree = (TTree*)file->Get("TofHitsTree");
    if (!tree) {
        std::cerr << "Error getting tree" << std::endl;
        file->Close();
        return;
    }

    int plane, bar, eventID;
    double x, y, z, signalTime, signalPosition;

    tree->SetBranchAddress("plane", &plane);
    tree->SetBranchAddress("bar", &bar);
    tree->SetBranchAddress("eventID", &eventID);
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);
    tree->SetBranchAddress("z", &z);
    tree->SetBranchAddress("signalTime", &signalTime);
    tree->SetBranchAddress("signalPosition", &signalPosition);

    std::ofstream outFile("output.txt");
    if (!outFile) {
        std::cerr << "Error creating output file" << std::endl;
        file->Close();
        return;
    }

    Long64_t entries = tree->GetEntries();
    if (entries == 0) {
        std::cerr << "No entries in tree" << std::endl;
    } else {
        std::cout << "Number of entries: " << entries << std::endl;
    }

    std::vector<std::pair<int, std::vector<std::tuple<double, double, double, int, int, double, double>>>> eventData;

    for (Long64_t i = 0; i < entries; i++) {
        tree->GetEntry(i);
        if (plane >= 0 && plane <= 5 && bar >= 1 && bar <= 20) {
            bool found = false;
            for (auto& ev : eventData) {
                if (ev.first == eventID) {
                    ev.second.emplace_back(std::make_tuple(x, y, z, plane, bar, signalPosition, signalTime));
                    found = true;
                    break;
                }
            }
            if (!found) {
                eventData.emplace_back(eventID, std::vector<std::tuple<double, double, double, int, int, double, double>>{std::make_tuple(x, y, z, plane, bar, signalPosition, signalTime)});
            }
        }
    }

    for (const auto& ev : eventData) {
        if (ev.second.size() >= 2) { // Đảm bảo có ít nhất hai điểm để tạo thành điểm bắt đầu và điểm kết thúc
            auto start = ev.second.front();
            auto end = ev.second.back();
            outFile << "EventID: " << ev.first << "\n";
            outFile << "Start: " << std::get<0>(start) << " " << std::get<1>(start) << " " << std::get<2>(start) << " "
                    << std::get<3>(start) << " " << std::get<4>(start) << " " << std::get<5>(start) << " " << std::get<6>(start) << "\n";
            outFile << "End: " << std::get<0>(end) << " " << std::get<1>(end) << " " << std::get<2>(end) << " "
                    << std::get<3>(end) << " " << std::get<4>(end) << " " << std::get<5>(end) << " " << std::get<6>(end) << "\n";
        }
    }

    outFile.close();
    file->Close();
}
