#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    } 
    else{
        PointerWrapper<AudioTrack> cloned_track = track.clone();
        if(!cloned_track){
            std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone";
            return 0; //??????????
        }
        cloned_track -> load();
        cloned_track -> analyze_beatgrid();

        bool evicted = cache.put(std::move(cloned_track));
        if(evicted){ return -1;}
        else{ return 0;}
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title); // Placeholder
}
