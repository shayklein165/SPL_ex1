#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

    DJLibraryService::~DJLibraryService() {
            for(AudioTrack* track: library){
            delete track;
        }
        library.clear();
    }
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    if(!library.empty()){
        for(AudioTrack* track: library){
            delete track;
        }
        library.clear();
    }

    for(const SessionConfig::TrackInfo& t : library_tracks){
        AudioTrack* new_track = nullptr;
        if(t.type=="MP3"){
            new_track = new MP3Track(t.title, t.artists, t.duration_seconds, t.bpm, t.extra_param1, t.extra_param2!=0);
            std::cout << "MP3: MP3Track created: " << t.extra_param1 << " kbps" << std::endl;
        }
        else if (t.type=="WAV"){
            new_track = new WAVTrack(t.title, t.artists, t.duration_seconds, t.bpm, t.extra_param1, t.extra_param2);
            std::cout << "WAVTrack created: " << t.extra_param1 << "Hz/" << t.extra_param2 << "bit" << std::endl;
        }

        if(new_track != nullptr){
            this->library.push_back(new_track); 
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;

}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return  playlist.find_track(track_title); // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    // For now, add a placeholder to fix the linker error
    //(void)playlist_name;  // Suppress unused parameter warning
    //(void)track_indices;  // Suppress unused parameter warning

    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    this -> playlist = Playlist(playlist_name);
    for(const int& i : track_indices){
        if (i < 1 || (size_t)i > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << i << std::endl;
            continue;
        }
        
        AudioTrack* canonical_track = this->library[i - 1];
        PointerWrapper<AudioTrack> cloned_wrapper = canonical_track->clone();
        
        if (!cloned_wrapper) {
            std::cout << "[ERROR] Track: \""<< canonical_track->get_title() << "\" failed to clone" << std::endl;
            continue;
        }

        AudioTrack* cloned_track = cloned_wrapper.release();
        cloned_track->load(); 
        cloned_track->analyze_beatgrid();

        this->playlist.add_track(cloned_track); 
        std::cout << "[INFO] Added '" << cloned_track->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
    }

    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << library.size() << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (const AudioTrack* track : tracks) {
        if (track != nullptr) {
            titles.push_back(track->get_title());
        }
    }
    return std::vector<std::string>(); // Placeholder
}
