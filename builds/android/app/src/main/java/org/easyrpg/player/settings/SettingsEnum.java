package org.easyrpg.player.settings;

import androidx.annotation.NonNull;

enum SettingsEnum {
    IMAGE_SIZE("ScalingMode"),
    VIBRATION_ENABLED("PREF_ENABLE_VIBRATION"),
    VIBRATE_WHEN_SLIDING_DIRECTION("PREF_VIBRATE_WHEN_SLIDING"),
    LAYOUT_TRANSPARENCY("PREF_LAYOUT_TRANSPARENCY"),
    IGNORE_LAYOUT_SIZE_SETTINGS("PREF_IGNORE_SIZE_SETTINGS"),
    LAYOUT_SIZE("PREF_SIZE_EVERY_BUTTONS"),
    EASYRPG_FOLDER_URI("PREF_EASYRPG_FOLDER_URI"),
    ENABLE_RTP_SCANNING("PREF_ENABLE_RTP_SCANNING"),
    SOUNDFONT_URI("Soundfont"),
    FAVORITE_GAMES("PREF_FAVORITE_GAMES_NEW"),
    CACHE_GAMES_HASH("PREF_CACHE_GAMES_HASH"),
    CACHE_GAMES("PREF_CACHE_GAMES"),
    FORCED_LANDSCAPE("PREF_FORCED_LANDSCAPE"),
    FAST_FORWARD_MODE("FAST_FORWARD_MODE"),
    INPUT_LAYOUT_HORIZONTAL("INPUT_LAYOUT_HORIZONTAL"),
    INPUT_LAYOUT_VERTICAL("INPUT_LAYOUT_VERTICAL"),
    MUSIC_VOLUME("MusicVolume"),
    SOUND_VOLUME("SoundVolume"),
    STRETCH("Stretch"),
    GAME_RESOLUTION("GameResolution"),
    SPEED_MODIFIER_A("SpeedModifierA"),
    FONT1_URI("Font1"),
    FONT2_URI("Font2"),
    FONT1_SIZE("Font1Size"),
    FONT2_SIZE("Font2Size")

    ;


    private final String label;

    SettingsEnum(String s) {
        this.label = s;
    }

    @NonNull
    @Override
    public String toString() {
        return label;
    }
}
