#ifndef PARTIAL_TRACKING_H
#define PARTIAL_TRACKING_H

#include "base.h"

extern "C" {
    #include "sms.h"
}

#include "SndObj.h"
#include "HammingTable.h"
#include "IFGram.h"
#include "SinAnal.h"

#include "Analyzer.h"
#include "AssociateBandwidth.h"
#include "BreakpointEnvelope.h"
#include "KaiserWindow.h"
#include "PartialBuilder.h"
#include "PartialList.h"
#include "ReassignedSpectrum.h"
#include "SpectralPeakSelector.h"

using namespace std;

namespace simpl
{


// ---------------------------------------------------------------------------
// PartialTracking
//
// Link spectral peaks from consecutive frames to form partials
// ---------------------------------------------------------------------------

class PartialTracking {
    protected:
        int _sampling_rate;
        int _max_partials;
        int _min_partial_length;
        int _max_gap;
        Frames _frames;

    public:
        PartialTracking();
        ~PartialTracking();

        void clear();

        int sampling_rate();
        virtual void sampling_rate(int new_sampling_rate);
        int max_partials();
        virtual void max_partials(int new_max_partials);
        int min_partial_length();
        virtual void min_partial_length(int new_min_partial_length);
        int max_gap();
        virtual void max_gap(int new_max_gap);

        virtual Peaks update_partials(Frame* frame);
        virtual Frames find_partials(Frames frames);
};


// ---------------------------------------------------------------------------
// SMSPartialTracking
// ---------------------------------------------------------------------------
class SMSPartialTracking : public PartialTracking {
    private:
        SMSAnalysisParams _analysis_params;
        SMSHeader _header;
        SMSData _data;
        sample* _peak_amplitude;
        sample* _peak_frequency;
        sample* _peak_phase;
        void init_peaks();

    public:
        SMSPartialTracking();
        ~SMSPartialTracking();
        void reset();
        void max_partials(int new_max_partials);
        bool realtime();
        void realtime(bool is_realtime);
        bool harmonic();
        void harmonic(bool is_harmonic);
        int max_frame_delay();
        void max_frame_delay(int new_max_frame_delay);
        int analysis_delay();
        void analysis_delay(int new_analysis_delay);
        int min_good_frames();
        void min_good_frames(int new_min_good_frames);
        bool clean_tracks();
        void clean_tracks(bool new_clean_tracks);
        Peaks update_partials(Frame* frame);
};


// ---------------------------------------------------------------------------
// SndObjPartialTracking
// ---------------------------------------------------------------------------
class SndObjPartialTracking : public PartialTracking {
    private:
        sample _threshold;
        int _num_bins;
        SndObj* _input;
        SinAnal* _analysis;
        sample* _peak_amplitude;
        sample* _peak_frequency;
        sample* _peak_phase;
        void reset();

    public:
        SndObjPartialTracking();
        ~SndObjPartialTracking();
        void max_partials(int new_max_partials);
        Peaks update_partials(Frame* frame);
};

// ---------------------------------------------------------------------------
// LorisPartialTracking
// ---------------------------------------------------------------------------
class SimplLorisPTAnalyzer : public Loris::Analyzer {
    protected:
        Loris::BreakpointEnvelope _env;
        Loris::PartialBuilder* _partial_builder;

    public:
        SimplLorisPTAnalyzer(int max_partials);
        ~SimplLorisPTAnalyzer();
        Loris::Peaks peaks;
        Loris::Peaks partials;
        void analyze();
};


class LorisPartialTracking : public PartialTracking {
    private:
        SimplLorisPTAnalyzer* _analyzer;

    public:
        LorisPartialTracking();
        ~LorisPartialTracking();
        void reset();
        void max_partials(int new_max_partials);
        Peaks update_partials(Frame* frame);
};


} // end of namespace simpl

#endif
