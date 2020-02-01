#define DEBUG

#include "state-common.h"


const stateCommon::LoopData empty = stateCommon::LoopData{
	.face= FACE_COUNT,
	.action=action::Action{.type=STATE_COMMON_ACTION_INVALID, .payload=0}
};

void setup() {
	
}

void loop() {
	stateCommon::LoopData data = empty;
	FOREACH_FACE(f) {
		if(isDatagramReadyOnFace(f)) {
			if(getDatagramLengthOnFace(f) != ACTION_LEN) {
				#ifdef DEBUG
                	setColor(RED);
				#endif
				return;
			}
			const byte* buffer = getDatagramOnFace(f);
			const action::Action a = action::decode(buffer);
			markDatagramReadOnFace(f);
			data = stateCommon::LoopData{.face = f, .action = a};
		}
	}
	stateCommon::loop(data);
}
