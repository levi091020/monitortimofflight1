void planetime()
{
    TChain *tIn = new TChain("TofHitsTree");
    tIn->Add("/home/lecamvi/Documents/TOF_thesis/output/run16489_outputTree.root");

    std::vector<Double_t> *SignalPosition = nullptr;
    std::vector<Double_t> *SignalPlane = nullptr;
    std::vector<Double_t> *SignalTime = nullptr;

    tIn->SetBranchAddress("SignalPosition", &SignalPosition);
    tIn->SetBranchAddress("SignalPlane", &SignalPlane);
    tIn->SetBranchAddress("SignalTime", &SignalTime);

    // Khởi tạo histogram
    TH1F *h_Top = new TH1F("h_Top", "Top Signal Position", 100,0, 100);
    TH1F *h_Bot = new TH1F("h_Bot", "Bottom Signal Position", 100,0, 100);
    TH1F *h_Up = new TH1F("h_Up", "Up Signal Position", 100,0, 100);
    TH1F *h_Down = new TH1F("h_Down", "Down Signal Position", 100,0, 100);

    Long64_t nEntries = tIn->GetEntries();

    // Khởi tạo mảng
    std::vector<Double_t> planeT(nEntries, 0.0);
    std::vector<Double_t> planeB(nEntries, 0.0);
    std::vector<Double_t> planeU(nEntries, 0.0);
    std::vector<Double_t> planeD(nEntries, 0.0);
    std::vector<Double_t> timeT(nEntries, 0.0);
    std::vector<Double_t> timeB(nEntries, 0.0);
    std::vector<Double_t> timeU(nEntries, 0.0);
    std::vector<Double_t> timeD(nEntries, 0.0);

    for (Long64_t i = 0; i < nEntries; i++)
    {
        tIn->GetEntry(i);

        // Tìm tọa độ x và thời gian của SignalPlane top, bot, up, và down
        for (size_t j = 0; j < SignalPosition->size(); j++)
        {
            if ((*SignalPlane)[j] == 3)
            {
                planeT[i] = (*SignalPosition)[j];
                timeT[i] = (*SignalTime)[j];
                h_Top->Fill(planeT[i]);
            }
            else if ((*SignalPlane)[j] == 2)
            {
                planeB[i] = (*SignalPosition)[j];
                timeB[i] = (*SignalTime)[j];
                h_Bot->Fill(planeB[i]);
            }
            else if ((*SignalPlane)[j] == 4)
            {
                planeU[i] = (*SignalPosition)[j];
                timeU[i] = (*SignalTime)[j];
                h_Up->Fill(planeU[i]);
            }
            else if ((*SignalPlane)[j] == 5)
            {
                planeD[i] = (*SignalPosition)[j];
                timeD[i] = (*SignalTime)[j];
                h_Down->Fill(planeD[i]);
            }
        }
    }

    // Tạo các histogram cho Time of Flight
    TH1F *h_timeOfFlight_TopBottom = new TH1F("h_timeOfFlight_TopBottom", "Time of Flight Top-Bottom", 40, -0.5, 55.5);
    TH1F *h_timeOfFlight_TopUpstream = new TH1F("h_timeOfFlight_UpstreamDownstream", "Time of Flight Top-Upstream", 40, -0.5, 55.5);
    TH1F *h_timeOfFlight_TopDownstream= new TH1F("h_timeOfFlight_NorthSouth", "Time of Flight Top-Downstream", 40, -0.5, 55.5);

    // Tính toán và vẽ Time of Flight
    for (Long64_t i = 0; i < nEntries; i++) {
        h_timeOfFlight_TopBottom->Fill(timeB[i] - timeT[i]);
        h_timeOfFlight_TopUpstream->Fill(timeU[i] - timeD[i]);
        h_timeOfFlight_TopDownstream->Fill(timeD[i] - timeT[i]);
    }

    // Vẽ các histogram trên cùng một canvas
    TCanvas *c_timeOfFlight = new TCanvas("c_timeOfFlight", "Time of Flight", 800, 600);
    c_timeOfFlight->cd();

    // Đặt các thuộc tính cho các histogram
    h_timeOfFlight_TopBottom->SetLineColor(kRed);
    h_timeOfFlight_TopUpstream->SetLineColor(kBlue);
    h_timeOfFlight_TopDownstream->SetLineColor(kGreen);

    h_timeOfFlight_TopBottom->Draw();
    h_timeOfFlight_TopUpstream->Draw("same");
    h_timeOfFlight_TopDownstream->Draw("same");

    // Thêm chú thích
    TLegend *legend = new TLegend(0.65, 0.65, 0.85, 0.85);
    legend->AddEntry(h_timeOfFlight_TopBottom, "Top-Bottom", "l");
    legend->AddEntry(h_timeOfFlight_TopUpstream, "Time of Flight Top-Upstream", "l");
    legend->AddEntry(h_timeOfFlight_TopDownstream, "Top-Downstream", "l");
    legend->Draw();

    c_timeOfFlight->SaveAs("TimeOfFlight.png");

    // Giải phóng bộ nhớ
    delete tIn;
    delete h_Top;
    delete h_Bot;
    delete h_Up;
    delete h_Down;
    delete h_timeOfFlight_TopBottom;
    delete h_timeOfFlight_TopUpstream;
    delete h_timeOfFlight_TopDownstream;
    delete c_timeOfFlight;
    delete legend;
}