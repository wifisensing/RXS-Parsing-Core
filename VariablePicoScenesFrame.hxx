//
// Created by csi on 12/17/19.
//

#ifndef PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
#define PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX

#include "rxs_enhanced.h"

struct ieee80211_mac_frame_header_frame_control_field {
    uint16_t version         :2,
            type            :2,
            subtype         :4,
            toDS            :1,
            fromDS          :1,
            moreFrags       :1,
            retry           :1,
            power_mgmt      :1,
            more            :1,
            protect         :1,
            order           :1;
} __attribute__ ((__packed__));

struct PicoScenesFrameHeader {
         uint8_t version :4,
         segments :4;
         uint8_t frameType;
         uint16_t taskId;
} __attribute__ ((__packed__));


#endif //PICOSCENES_PLATFORM_VARIABLEPICOSCENESFRAME_HXX
