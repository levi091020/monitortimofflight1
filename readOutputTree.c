#include <TTree.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

// Hằng số hình học (điều chỉnh các giá trị này cho phù hợp với thiết lập của bạn)
const double bar_width = 2.5; // Chiều rộng của một thanh
const double bar_length = 120; // Chiều dài của một thanh
const double bar_distance = 3.1; // Khoảng cách giữa các thanh
const int bars_per_plane = 18; // Số lượng thanh trên mỗi mặt phẳng
const double offset = 111.25; // Độ dịch chuyển

// Hàm chuyển đổi tọa độ
bool getCoordinate(int Plane, int Bar, double coord, double &x, double &y, double &z) {
    // Kiểm tra tính hợp lệ của Plane và Bar
    if (Plane < 0 || Plane > 5) {
        std::cerr << "Invalid Plane value: " << Plane << std::endl;
        return false;
    }
    if (Bar < 0 || Bar > 19) {
        std::cerr << "Invalid Bar value: " << Bar << std::endl;
        return false;
    }

    // Xử lý trường hợp Plane = 0 hoặc 1 (North và South)
    if (Plane == 0 || Plane == 1) {
        std::cerr << "North and South modules not installed." << std::endl;
        return false;
    }

    // Thiết lập hình học cho từng mặt phẳng
    if (Plane == 2) { // Bottom
        x = -offset + bar_distance * Bar;
        y = -bar_length / 2.0 + coord;
        z = 0;
    } else if (Plane == 3) { // Top
        x = -offset + bar_distance * Bar;
        y = bar_length / 2.0 - coord;
        z = 0;
    } else if (Plane == 4) { // Upstream
        x = -bar_length / 2.0 + coord;
        y = offset - bar_distance * Bar;
        z = 0;
    } else if (Plane == 5) { // Downstream
        x = bar_length / 2.0 - coord;
        y = offset - bar_distance * Bar;
        z = 0;
    }

    // Xử lý Ghost Bar
    if ((Plane == 2 || Plane == 3 || Plane == 4 || Plane == 5) && (Bar == 0 || Bar == 19)) {
        std::cerr << "Ghost Bar detected on Plane: " << Plane << ", Bar: " << Bar << std::endl;
        x = y = z = -9999; // Hoặc giá trị khác để biểu thị lỗi
        return false;
    }

    return true;
}

// Hàm đọc cây TTree và ghi dữ liệu vào tệp
void readOutputTree() {
    // Mở file ROOT
    TFile *file = new TFile("/home/lecamvi/Documents/TOF_thesis/output/run2134_outputTree.root", "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening ROOT file." << std::endl;
        exit(1); 
    }

    // Lấy cây TTree "Plane"
    TTree *tree = (TTree*)file->Get("TofHitsTree");
    if (!tree) {
        std::cerr << "Error getting TTree 'TofHitsTree'." << std::endl;
        file->Close();
        exit(1);
    }

    // Lấy các nhánh từ cây
    int plane, bar;
    double x;

    tree->SetBranchAddress("Plane", &plane);
    tree->SetBranchAddress("Bar", &bar);
    tree->SetBranchAddress("X", &x);

    double x_start, y_start, z_start, x_end, y_end, z_end;

    // Mở file output
    std::ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        file->Close();
        exit(1);
    }

    Long64_t entries = tree->GetEntries();
    for (Long64_t i = 0; i < entries; i++) { 
        tree->GetEntry(i);

        if (getCoordinate(plane, bar, x - 110, x_start, y_start, z_start) &&
            getCoordinate(plane, bar, x + 110, x_end, y_end, z_end)) {
            outFile << plane << " " << bar << " " << std::setprecision(6) << x_start << " "
                    << y_start << " " << z_start << " " << x_end << " " << y_end << " " << z_end << std::endl;
        } else {
            outFile << "Coordinate conversion failed for entry " << i << std::endl;
        }
    }

    outFile.close();
    file->Close();
}

int main() {
    readOutputTree();
    return 0;
}
