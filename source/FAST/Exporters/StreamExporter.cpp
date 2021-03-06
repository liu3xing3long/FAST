#include "StreamExporter.hpp"
#include "FAST/Utility.hpp"

namespace fast {

void StreamExporter::setFilenameFormat(std::string format) {
    mFilenameFormat = format;
}

void StreamExporter::setExporter(SharedPointer<FileExporter> exporter) {
    mExporter = exporter;
}

StreamExporter::StreamExporter() {
    createInputPort<DataObject>(0, INPUT_DYNAMIC);
    mFilenameFormat = "";
    mFrameCounter = 0;
    mFinished = false;
}

bool StreamExporter::isFinished() {
    return mFinished;
}

void StreamExporter::execute() {
    if(mFilenameFormat == "")
        throw Exception("Filename format must be given to StreamExporter");

    if(mFilenameFormat.find("#") == std::string::npos)
        throw Exception("Filename format must contain # which will be replaced by frame number");

    std::string filename = replace(mFilenameFormat, "#", std::to_string(mFrameCounter));
    reportInfo() << "Exporting " << filename << reportEnd();

    DataObject::pointer data = getStaticInputData<DataObject>();
    mExporter->setInputData(data);
    mExporter->setFilename(filename);
    mExporter->update();
    mFrameCounter++;
    DynamicData::pointer dynamicData = getInputData(0);
    if(dynamicData->hasReachedEnd())
        mFinished = true;
}

}