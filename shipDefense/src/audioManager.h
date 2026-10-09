#pragma once

#include "raylib.h"

#include <array>
#include <utility>

// TODO update the terrible sounds

class AudioManager
{
public:
    enum class Effect
    {
        EnemyExplosion,
        EnemyShoot,
        PlayerShoot,
        RadarPing,
        ChooseUpgrade,
        ConfirmClick,
        LevelUp,
        Blip
    };

    AudioManager()
    {
        InitAudioDevice();

        effects = {
            LoadSound("assets/audio/effects/enemyExplosion.mp3"),
            LoadSound("assets/audio/effects/enemyShoot.wav.wav"),
            LoadSound("assets/audio/effects/playerShoot.wav.wav"),
            LoadSound("assets/audio/effects/radarPing.wav"),
            LoadSound("assets/audio/effects/chooseUpgrade.aiff"),
            LoadSound("assets/audio/effects/confirmClick.wav"),
            LoadSound("assets/audio/effects/levelUp.wav"),
            LoadSound("assets/audio/effects/blip.wav")};
        music = {
            LoadMusicStream("assets/audio/music/1.mp3"),
            LoadMusicStream("assets/audio/music/2.mp3"),
            LoadMusicStream("assets/audio/music/3.mp3"),
            LoadMusicStream("assets/audio/music/4.mp3")};

        for (Music &track : music)
            track.looping = false;

        SetSoundVolume(effects[(int)Effect::RadarPing], 0.2f);
        SetSoundVolume(effects[(int)Effect::EnemyShoot], 0.3f);
        SetSoundVolume(effects[(int)Effect::PlayerShoot], 0.5f);

        shuffleMusic();
        playNextTrack();
    }

    ~AudioManager()
    {
        for (Sound &effect : effects)
            UnloadSound(effect);
        for (Music &track : music)
            UnloadMusicStream(track);

        CloseAudioDevice();
    }

    void update()
    {
        if (currentTrack < 0)
            return;

        UpdateMusicStream(music[currentTrack]);
        if (!IsMusicStreamPlaying(music[currentTrack]))
            playNextTrack();
    }

    void playEffect(Effect effect)
    {
        PlaySound(effects[(int)effect]);
    }

private:
    void shuffleMusic()
    {
        for (int index = static_cast<int>(musicOrder.size()) - 1; index > 0; --index)
        {
            const int swapIndex = GetRandomValue(0, index);
            std::swap(musicOrder[index], musicOrder[swapIndex]);
        }
        nextTrack = 0;
    }

    void playNextTrack()
    {
        if (nextTrack == musicOrder.size())
            shuffleMusic();

        currentTrack = static_cast<int>(musicOrder[nextTrack++]);
        PlayMusicStream(music[currentTrack]);
    }

    std::array<Sound, 8> effects{};
    std::array<Music, 4> music{};
    std::array<std::size_t, 4> musicOrder{0, 1, 2, 3};
    std::size_t nextTrack = 0;
    int currentTrack = -1;
};