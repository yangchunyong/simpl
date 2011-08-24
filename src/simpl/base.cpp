/*
 * Copyright (c) 2009-2011 John Glover, National University of Ireland, Maynooth
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "base.h"

using namespace std;

namespace Simpl {

// ---------------------------------------------------------------------------
//	Peak
// ---------------------------------------------------------------------------
Peak::Peak()
{
    amplitude = 0.0;
    frequency = 0.0;
    phase = 0.0;
    next_peak = NULL;
    previous_peak = NULL;
    partial_id = 0;
    partial_position = 0;
    frame_number = 0;
}

Peak::~Peak()
{
}

// Returns true iff this peak is unmatched in the given direction, and has positive amplitude
bool Peak::is_free(string direction)
{
    if(amplitude <= 0.0)
    {
        return false;
    }

    if(direction == "forwards")
    {
        if(next_peak != NULL)
        {
            return false;
        }
    }
    else if(direction == "backwards")
    {
        if(previous_peak != NULL)
        {
            return false;
        }
    }
    else
    {
		Throw(InvalidArgument, "Invalid direction");
    }

    return true;
}

// ---------------------------------------------------------------------------
//	Frame
// ---------------------------------------------------------------------------
Frame::Frame()
{
    _size = 512;
    init();
}

Frame::Frame(int frame_size)
{
    _size = frame_size;
    init();
}

Frame::~Frame()
{
}

void Frame::init()
{
    _max_peaks = 100;
    peaks.resize(_max_peaks);
    _max_partials = 100;
    partials.resize(_max_partials);
}

int Frame::size()
{
    return _size;
}

void Frame::size(int new_size)
{
    _size = new_size;
}

int Frame::max_peaks()
{
    return _max_peaks;
}

void Frame::max_peaks(int new_max_peaks)
{
    _max_peaks = new_max_peaks;
    peaks.resize(_max_peaks);
}

int Frame::max_partials()
{
    return _max_partials;
}

void Frame::max_partials(int new_max_partials)
{
    _max_partials = new_max_partials;
    partials.resize(_max_partials);
}

// ---------------------------------------------------------------------------
//	PeakDetection
// ---------------------------------------------------------------------------

PeakDetection::PeakDetection()
{
    _sampling_rate = 44100;
    _frame_size = 2048;
    _static_frame_size = true;
    _hop_size = 512;
    _max_peaks = 100;
    _window_type = "hamming";
    _window_size = 2048;
    _min_peak_separation = 1.0; // in Hz
}

PeakDetection::~PeakDetection()
{
}

int PeakDetection::sampling_rate()
{
    return _sampling_rate;
}

void PeakDetection::sampling_rate(int new_sampling_rate)
{
    _sampling_rate = new_sampling_rate;
}

int PeakDetection::frame_size()
{
    return _frame_size;
}
void PeakDetection::frame_size(int new_frame_size)
{
    _frame_size = new_frame_size;
}

bool PeakDetection::static_frame_size()
{
    return _static_frame_size;
}

void PeakDetection::static_frame_size(bool new_static_frame_size)
{
    _static_frame_size = new_static_frame_size;
}

int PeakDetection::next_frame_size()
{
    return _frame_size;
}

int PeakDetection::hop_size()
{
    return _hop_size;
}

void PeakDetection::hop_size(int new_hop_size)
{
    _hop_size = new_hop_size;
}

int PeakDetection::max_peaks()
{
    return _max_peaks;
}

void PeakDetection::max_peaks(int new_max_peaks)
{
    _max_peaks = new_max_peaks;
}

std::string PeakDetection::window_type()
{
    return _window_type;
}

void PeakDetection::window_type(std::string new_window_type)
{
    _window_type = new_window_type;
}

int PeakDetection::window_size()
{
    return _window_size;
}

void PeakDetection::window_size(int new_window_size)
{
    _window_size = new_window_size;
}

number PeakDetection::min_peak_separation()
{
    return _min_peak_separation;
}

void PeakDetection::min_peak_separation(number new_min_peak_separation)
{
    _min_peak_separation = new_min_peak_separation;
}

Frames* PeakDetection::frames()
{
    return &_frames;
}

} // end of namespace Simpl