#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <iostream>

// Định nghĩa cấu trúc để lưu trữ dữ liệu time-of-flight của sự kiện
struct Position {
    double x;
    double y;
    double z;
    int plane;
    double time;
    double position;
    int bar;
};

struct Event {
    Position start;
    Position end;
};

// Hàm chuyển đổi tọa độ từ file analysisPrime.pdf
bool getCoordinate(int Plane, int Bar, double coord, double &x, double &y, double &z);

void extractEventTimeOfFlight() {
    // Khai báo các biến SignalTime và SignalPosition
    double SignalTime, SignalPosition;

    // Khai báo biến EventTimeOfFlight
    Event Event;

    // Mở tệp dữ liệu
    TFile* file = new TFile("/home/lecamvi/Documents/TOF_thesis/output/run16480_outputTree.root", "READ");

    // Lấy TTree từ tệp
    TTree* tree = (TTree*)file->Get("TofHitsTree");
    
    double startX, startY, startZ, endX, endY, endZ;
    int Plane, Bar;

    // Thiết lập địa chỉ nhánh cho các biến
    tree->Branch("start_x", &Event.start.x, "start_x/D");
    tree->Branch("start_y", &Event.start.y, "start_y/D");
    tree->Branch("start_z", &Event.start.z, "start_z/D");
    tree->Branch("start_plane", &Event.start.plane, "start_plane/I");
    tree->Branch("end_x", &Event.end.x, "end_x/D");
    tree->Branch("end_y", &Event.end.y, "end_y/D");
    tree->Branch("end_z", &Event.end.z, "end_z/D");
    tree->Branch("end_plane", &Event.end.plane, "end_plane/I");
    tree->Branch("start_time", &Event.start.time, "start_time/D");
    tree->Branch("end_time", &Event.end.time, "end_time/D");
    tree->Branch("start_position", &Event.start.position, "start_position/D");
    tree->Branch("end_position", &Event.end.position, "end_position/D");
    tree->Branch("start_bar", &Event.start.bar, "start_bar/I");
    tree->Branch("end_bar", &Event.end.bar, "end_bar/I");

    // Lặp qua mỗi entry trong TTree và in ra giá trị của x, y, z, plane, bar, time và position
    int entries = tree->GetEntries();
    for (int i = 0; i < entries; i++) {
        tree->GetEntry(i);
        std::cout << "Start: x = " << Event.start.x << ", y = " << Event.start.y << ", z = " << Event.start.z 
                  << ", plane = " << Event.start.plane << ", bar = " << Event.start.bar
                  << ", time = " << Event.start.time << ", position = " << Event.start.position << std::endl;
        std::cout << "End: x = " << Event.end.x << ", y = " << Event.end.y << ", z = " << Event.end.z
                  << ", plane = " << Event.end.plane << ", bar = " << Event.end.bar
                  << ", time = " << Event.end.time << ", position = " << Event.end.position << std::endl;
    }
    // Đóng tệp
    file->Close();
}
