/*
 * PhysicalQueue.h
 *
 *  Created on: 2024. 01. 21.
 *      Author: Suhwan
 */

#ifndef __DDC_PHYSICALBACKLOG_H_
#define __DDC_PHYSICALBACKLOG_H_

#include <iostream>
#include <queue>
#include <omnetpp.h>

#include "inet/queueing/queue/PacketQueue.h"

#include "ddc/message/Compute_m.h"

#include "ddc/processingunit/ProcessingUnit.h"

using namespace std;
using namespace inet::queueing;

namespace inet{
    class PhysicalQueue {
        private:
            PacketQueue *link_physical_queue;

        public:
            PhysicalQueue();

            void initLinkPhysicalQueue(PacketQueue *link_physical_queue);

            bool isPhysicalQueueExist();
            bool isLinkPhysicalQueueExist();

            double getQueueLength();
            double getLinkQueueLength();

    };
}


#endif /* __DDC_PHYSICALBACKLOG_H_ */
