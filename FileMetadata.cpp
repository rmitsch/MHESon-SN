/* 
 * File:   FileSettings.cpp
 * Author: RM
 * 
 * Created on 29. Juli 2013, 22:34
 */

#include "FileMetadata.h"

FileMetadata::FileMetadata()
{
}

FileMetadata::FileMetadata  (string inputPath, FileMetadataConstants::DataFileType fileType, uint sizeEstimation)
                        :   _inputPath(inputPath), _fileType(fileType), _sizeEstimation(sizeEstimation)
{
}
        
FileMetadata::FileMetadata(const FileMetadata& orig)
{
}

FileMetadata::~FileMetadata()
{
}

string& FileMetadata::inputPath()
{
    return _inputPath;
}

string& FileMetadata::fileName()
{
    return _fileName;
}

FileMetadataConstants::DataFileType& FileMetadata::fileType()
{
    return _fileType;
}

FileMetadataConstants::FileSpecification& FileMetadata::fileSpecification()
{
    return _fileSpecification;
}

uint& FileMetadata::sizeEstimation()
{
    return _sizeEstimation;
}

bool& FileMetadata::hasNativeEdgeWeights()
{
    return _hasNativeEdgeWeights;
}

bool& FileMetadata::isDirected()
{
    return _isDirected;
}

void FileMetadata::loadSettings(FileMetadataConstants::FileSpecification fileSpecification)
{
    switch (fileSpecification) {
        // --- Settings for Epinions ---
        // Source: http://snap.stanford.edu/data/soc-Epinions1.html
        case FileMetadataConstants::EPINIONS_F:
            _inputPath              = "../resources/Epinions/soc-Epinions1_inverted.txt";
            _fileName               = "Epinions";
            _fileType               = FileMetadataConstants::SNAP;
            _fileSpecification      = FileMetadataConstants::EPINIONS_F;
            _sizeEstimation         = 75879;
            // Total edge count: 508837
            _averageEdgeCount       = 508837 / _sizeEstimation;
            _hasNativeEdgeWeights   = false;
            _isDirected             = true;
        break;
        // -----------------------------

        // --- Settings for Slashdot ---
        // Source: http://snap.stanford.edu/data/soc-Slashdot0902.html
        case FileMetadataConstants::SLASHDOT_F:
            _inputPath              = "../resources/Slashdot/Slashdot0902_inverted.txt";
            _fileName               = "Slashdot0902";
            _fileType               = FileMetadataConstants::SNAP;
            _fileSpecification      = FileMetadataConstants::SLASHDOT_F;
            _sizeEstimation         = 82168;
            // Total edge count: 948464
            _averageEdgeCount       = 948464 / _sizeEstimation;
            _hasNativeEdgeWeights   = false;
            _isDirected             = true;
        break;
        // -----------------------------
        
        // --- Settings for ADVOGATO ---
        // Source: http://konect.uni-koblenz.de/networks/advogato
        case FileMetadataConstants::ADVOGATO_F:
            _inputPath              = "../resources/Advogato/out.advogato_inverted_weighted";
            _fileName               = "Advogato";
            _fileType               = FileMetadataConstants::ADVOGATO;
            _fileSpecification      = FileMetadataConstants::ADVOGATO_F;
            _sizeEstimation         = 6551;
            // Total edge count: 51332
            _averageEdgeCount       = 51332 / _sizeEstimation;
            _hasNativeEdgeWeights   = true;
            _isDirected             = true;
        break;
        // -----------------------------
        
        // --- Settings for DBLP ---
        // Source: http://konect.uni-koblenz.de/networks/dblp_coauthor
        case FileMetadataConstants::DBLP_F:
            _inputPath              = "../resources/DBLP/text/out.dblp_coauthor_inverted_reduced_weighted";
            _fileName               = "DBLP";
            _fileType               = FileMetadataConstants::DBLP;
            _fileSpecification      = FileMetadataConstants::DBLP_F;
            _sizeEstimation         = 1248427;
            // Total edge count: 17631144
            _averageEdgeCount       = 17631144 / _sizeEstimation; 
            _hasNativeEdgeWeights   = true;
            _isDirected             = false;
        break;        
        // -----------------------------
    }
}