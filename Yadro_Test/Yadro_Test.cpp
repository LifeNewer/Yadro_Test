#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

class TapeInterface {
public:
    virtual void write(int data) = 0;
    virtual int read() = 0;
    virtual void rewind() = 0;
    virtual void shift() = 0;
    virtual bool eof() const = 0;
};

class FileTape : public TapeInterface {
public:
    FileTape(const std::string& filename, int writeDelay, int readDelay, int rewindDelay, int shiftDelay) :
        writeDelay(writeDelay), readDelay(readDelay), rewindDelay(rewindDelay), shiftDelay(shiftDelay)
    {
        file.open(filename);
        outputFile.open(filename + ".out");
    }

    ~FileTape() {
        file.close();
        outputFile.close();
    }

    void write(int data) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(writeDelay));
        outputFile << data << '\n';
    }

    int read() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(readDelay));
        int data;
        file >> data;
        return data;
    }

    void rewind() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(rewindDelay));
        file.clear();
        file.seekg(0, std::ios::beg);
    }

    void shift() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(shiftDelay));
        file.seekg(0, std::ios::cur);
    }

    bool isFileOpen() const {
        return file.is_open();
    }

    bool eof() const override {
        return file.eof();
    }

private:
    std::ifstream file;
    std::ofstream outputFile;
    int writeDelay;
    int readDelay;
    int rewindDelay;
    int shiftDelay;
};

class TapeSorter {
public:
    TapeSorter(TapeInterface& inputTape, TapeInterface& outputTape) :
        inputTape(inputTape), outputTape(outputTape) {}

    void sort() {
        std::vector<int> buffer;

        
        while (!inputTape.eof()) {
            int data = inputTape.read();
            buffer.push_back(data);
        }

        
        std::sort(buffer.begin(), buffer.end());

        
        for (int data : buffer) {
            outputTape.write(data);
        }
    }

private:
    TapeInterface& inputTape;
    TapeInterface& outputTape;
};

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

int main() {
    
    int writeDelay = 100;
    int readDelay = 100;
    int rewindDelay = 500;
    int shiftDelay = 100;

    std::string inputTapeFilename = "input_tape.txt";
    std::string outputTapeFilename = "output_tape.txt";

    
    if (!fileExists(inputTapeFilename)) {
        std::cout << "Input tape file not found or could not be opened!" << std::endl;
        return 1; 
    }

    FileTape inputTape(inputTapeFilename, writeDelay, readDelay, rewindDelay, shiftDelay);
    FileTape outputTape(outputTapeFilename, writeDelay, readDelay, rewindDelay, shiftDelay);

    TapeSorter tapeSorter(inputTape, outputTape);
    tapeSorter.sort();

    std::cout << "Sorting completed!" << std::endl;

    return 0;
}
