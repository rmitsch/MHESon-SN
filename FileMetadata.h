/* 
 * File:   FileMetadata.h
 * Author: RM
 *
 * Created on 29. Juli 2013, 22:34
 */

#ifndef FILEMETADATA_H
#define	FILEMETADATA_H

#include <iostream>
#include <string>

#include "Definitions.h"
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

namespace FileMetadataConstants
{
    enum DataFileType       {DBLP, ADVOGATO, SNAP};
    enum FileSpecification  {DBLP_F, ADVOGATO_F, EPINIONS_F, SLASHDOT_F};
}

class FileMetadata
{
    public:
        FileMetadata(string inputPath, FileMetadataConstants::DataFileType fileType, uint sizeEstimation);
        FileMetadata();
        FileMetadata(const FileMetadata& orig);
        virtual ~FileMetadata();
        
        string&                                     inputPath();
        string&                                     fileName();
        FileMetadataConstants::DataFileType&        fileType();
        FileMetadataConstants::FileSpecification&   fileSpecification();
        uint&                                       sizeEstimation();
        uint&                                       averageEdgeCount();
        bool&                                       hasNativeEdgeWeights();
        bool&                                       isDirected();
        
        void                                        loadSettings(FileMetadataConstants::FileSpecification fileSpecification);
    
    private:
        string                                      _inputPath;
        string                                      _fileName;        
        FileMetadataConstants::DataFileType         _fileType;
        FileMetadataConstants::FileSpecification    _fileSpecification;
        uint                                        _sizeEstimation;
        uint                                        _averageEdgeCount;
        bool                                        _hasNativeEdgeWeights;
        bool                                        _isDirected;
} ;

#endif	/* FILEMETADATA_H */

