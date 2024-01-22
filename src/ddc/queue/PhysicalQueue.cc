/*
 * PhysicalQueue.cc
 *
 *  Created on: 2024. 01. 21.
 *      Author: Suhwan
 */

#include "ddc/queue/PhysicalQueue.h"

namespace inet{
    PhysicalQueue::PhysicalQueue(){}

    void PhysicalQueue::initLinkPhysicalQueue(PacketQueue *link_physical_queue) {
        this->link_physical_queue = link_physical_queue;
    }

    bool PhysicalQueue::isPhysicalQueueExist() {
        if (isLinkPhysicalQueueExist()) { return true; }
        else { return false; }
    }

    bool PhysicalQueue::isLinkPhysicalQueueExist() {
        if (this->link_physical_queue) { return true; }
        else { return false; }
    }

    double PhysicalQueue::getQueueLength() {
        return getLinkQueueLength();
    }

    double PhysicalQueue::getLinkQueueLength() {
        return this->link_physical_queue->getTotalLength().get();
    }

}
