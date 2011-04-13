/*
 * This file is part of bino, a 3D video player.
 *
 * Copyright (C) 2011
 * Martin Lambers <marlam@marlam.de>
 * Joe <cuchac@email.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SUBTITLE_RENDERER_H
#define SUBTITLE_RENDERER_H

#include <vector>

#include <GL/glew.h>

extern "C"
{
#include <ass/ass.h>
}

#include "media_data.h"


class subtitle_renderer
{
private:
    // Static ASS data
    bool _ass_initialized;
    const char *_fontconfig_conffile;
    ASS_Library *_ass_library;
    ASS_Renderer *_ass_renderer;

    // Dynamic data (changes with each subtitle)
    subtitle_box::format_t _fmt;
    ASS_Track *_ass_track;
    ASS_Image *_ass_img;
    const subtitle_box *_img_box;
    int _bb_x, _bb_y, _bb_w, _bb_h;

    // ASS helper functions
    const char *get_fontconfig_conffile();
    void init_ass();
    void blend_ass_image(const ASS_Image *img, uint32_t *buf);
    void set_ass_parameters(const parameters &params);

    // Rendering ASS and text subtitles
    void prerender_ass(const subtitle_box &box, int64_t timestamp,
            const parameters &params,
            int width, int height, float pixel_aspect_ratio);
    void render_ass(uint32_t *bgra32_buffer);

    // Rendering bitmap subtitles
    void prerender_img(const subtitle_box &box);
    void render_img(uint32_t *bgra32_buffer);

public:
    subtitle_renderer();
    ~subtitle_renderer();

    /*
     * To render a subtitle, the following steps are necessary:
     * 1. Call render_to_display_size() to determine if the subtitle
     *    overlay image should have display size or video frame size.
     * 2. Call prerender() to determine the bounding box inside the
     *    overlay image that the subtitle will occupy.
     * 3. Clear the overlay image, and allocate a BGRA32 buffer for
     *    the bounding box.
     * 4. Call render() to draw the subtitle into the buffer.
     * 5. Update the overlay image with the subtitle bounding box
     *    image from the buffer.
     */

    // Return true if the subtitle should be rendered in display resolution.
    // Return false if the subtitle should be rendered in video frame resolution.
    bool render_to_display_size(const subtitle_box &box) const;

    // Prerender the subtitle, to determine the bounding box it will occupy.
    // The bounding box is relative to the given subtitle overlay size (width and height).
    void prerender(const subtitle_box &box, int64_t timestamp,
            const parameters &params,
            int width, int height, float pixel_aspect_ratio,
            int &bb_x, int &bb_y, int &bb_w, int &bb_h);

    // Render the prerendered subtitle into the given BGRA32 buffer, which must
    // have the dimensions of the bounding box that was previously computed.
    void render(uint32_t *bgra32_buffer);
};

#endif