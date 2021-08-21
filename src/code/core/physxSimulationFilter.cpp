#include "mypch.h"
#include "core/application.h"
#include "assets/rigidbody.h"

#include "core/physxSimulationFilter.h"

void PhysxSimulatorCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    for(PxU32 i=0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];

        if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            //Log("Actor 1:  " << pairHeader.actors[0]->getName());
            //Log("Actor 2:  " << pairHeader.actors[1]->getName());

            const auto& entity1 = Application::GetSceneModifiable().GetEntityWithUUID(pairHeader.actors[0]->getName());
            const auto& entity2 = Application::GetSceneModifiable().GetEntityWithUUID(pairHeader.actors[1]->getName());

            entity1->OnCollision(entity2);
            entity2->OnCollision(entity1);

            /*
            if((pairHeader.actors[0] == mSubmarineActor) ||
                    (pairHeader.actors[1] == mSubmarineActor))
            {
                PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ?
                                      pairHeader.actors[1] : pairHeader.actors[0];
                Seamine* mine =  reinterpret_cast<Seamine*>(otherActor->userData);
                // insert only once
                if(std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) ==
                        mMinesToExplode.end())
                    mMinesToExplode.push_back(mine);

                break;
            }
            */

        }
    }
}
void PhysxSimulatorCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
    for(PxU32 i=0; i < count; i++)
    {
        // ignore pairs when shapes have been deleted
        if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)) {
            continue;
        }
    }
}
void PhysxSimulatorCallback::onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count) {
    Log("WOT!!!");
}
void PhysxSimulatorCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {
    Log("BREAK!!!");
}
void PhysxSimulatorCallback::onWake(PxActor** actors, PxU32 count) {
    Log("WAKE!!!");
}
void PhysxSimulatorCallback::onSleep(PxActor** actors, PxU32 count) {
    Log("SLEEP!!!");
}
auto SampleSubmarineFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) -> PxFilterFlags
{
    if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
        return PxFilterFlag::eDEFAULT;
    }

    if(((filterData0.word0 & filterData1.word1) != 0u) && ((filterData1.word0 & filterData0.word1) != 0u)) {
        pairFlags |= PxPairFlag::eCONTACT_DEFAULT;
    }

    if(((filterData0.word0 & filterData1.word2) != 0u) && ((filterData1.word0 & filterData0.word2) != 0u)) {
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
    }

    return PxFilterFlag::eDEFAULT;
}
