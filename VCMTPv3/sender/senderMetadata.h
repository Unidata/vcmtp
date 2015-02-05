/**
 * Copyright (C) 2014 University of Virginia. All rights reserved.
 *
 * @file      senderMetadata.h
 * @author    Shawn Chen <sc7cq@virginia.edu>
 * @version   1.0
 * @date      Nov 28, 2014
 *
 * @section   LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or（at your option）
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details at http://www.gnu.org/copyleft/gpl.html
 *
 * @brief     Define the interfaces and structures of VCMTPv3 sender side
 *            retransmission metadata.
 *
 * VCMTPv3 sender side retransmission data structures and interfaces, including
 * the timeout period value, unfinished set and product pointers.
 */



#ifndef SENDERMETADATA_H_
#define SENDERMETADATA_H_

#include <set>
#include <map>
#include <pthread.h>
#include <stdint.h>


struct RetxMetadata {
    uint32_t  prodindex;
    /** recording the whole product size (for timeout factor use) */
    uint32_t  prodLength;
    uint16_t  metaSize;          /*!< metadata size */
    void*     metadata;          /*!< metadata pointer */
    clock_t   mcastStartTime;    /*!< multicasting start time */
    clock_t   mcastEndTime;      /*!< multicasting end time */
    float     retxTimeoutRatio;  /*!< ratio to scale timeout time */
    float     retxTimeoutPeriod; /*!< timeout time in seconds */
    void*     dataprod_p;        /*!< pointer to the data product */
    /** unfinished receiver set indexed by socket id */
    std::set<int>  unfinReceivers;

    RetxMetadata(): prodindex(0), prodLength(0), metaSize(0),
                    metadata(NULL), mcastStartTime(0.0),
                    mcastEndTime(0.0), retxTimeoutRatio(20.0),
                    retxTimeoutPeriod(99999999999.0), dataprod_p(NULL) {}
    virtual ~RetxMetadata() {}
};


class senderMetadata {
public:
    senderMetadata();
    ~senderMetadata();

    void addRetxMetadata(RetxMetadata* ptrMeta);
    bool rmRetxMetadata(uint32_t prodindex);
    RetxMetadata* getMetadata(uint32_t prodindex);
    bool clearUnfinishedSet(uint32_t prodindex, int retxsockfd);

private:
    /** first: prodindex; second: pointer to metadata of the specified prodindex */
    std::map<uint32_t, RetxMetadata*> indexMetaMap;
    pthread_rwlock_t                  indexMetaMapLock;
    bool rmRetxMetadataNoLock(uint32_t prodindex);
};

#endif /* SENDERMETADATA_H_ */
