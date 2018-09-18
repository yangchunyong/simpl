import os
import json
from nose.tools import assert_almost_equals
import simpl
import simpl.peak_detection as peak_detection
import simpl.partial_tracking as partial_tracking

PeakDetection = peak_detection.PeakDetection
SMSPeakDetection = peak_detection.SMSPeakDetection
SndObjPeakDetection = peak_detection.SndObjPeakDetection
PartialTracking = partial_tracking.PartialTracking
SMSPartialTracking = partial_tracking.SMSPartialTracking
SndObjPartialTracking = partial_tracking.SndObjPartialTracking

float_precision = 2
frame_size = 512
hop_size = 512
max_peaks = 10
max_partials = 10
num_frames = 30
num_samples = num_frames * hop_size
audio_path = os.path.join(
    os.path.dirname(__file__), 'audio/flute.wav'
)
libsms_test_data_path = os.path.join(
    os.path.dirname(__file__), 'libsms_test_data.json'
)
sndobj_test_data_path = os.path.join(
    os.path.dirname(__file__), 'sndobj_test_data.json'
)


def _load_libsms_test_data():
    test_data = None
    with open(libsms_test_data_path, 'r') as f:
        test_data = json.loads(f.read())
    return test_data


def _load_sndobj_test_data():
    test_data = None
    with open(sndobj_test_data_path, 'r') as f:
        test_data = json.loads(f.read())
    return test_data


class TestPartialTracking(object):
    @classmethod
    def setup_class(cls):
        cls.audio = simpl.read_wav(audio_path)[0]
        cls.audio = cls.audio[0:num_samples]

    def test_basic(self):
        pd = PeakDetection()
        pd.hop_size = hop_size
        frames = pd.find_peaks(self.audio)

        pt = PartialTracking()
        frames = pt.find_partials(frames)

        assert len(frames[0].partials) == 0
        assert frames[0].max_partials == 100


class TestSMSPartialTracking(object):
    @classmethod
    def setup_class(cls):
        cls.audio = simpl.read_wav(audio_path)[0]
        cls.audio = cls.audio[0:num_samples]
        cls.test_data = _load_libsms_test_data()

    def test_basic(self):
        pd = SMSPeakDetection()
        pd.hop_size = hop_size
        pd.frame_size = hop_size
        pd.max_peaks = max_peaks
        pd.static_frame_size = True
        frames = pd.find_peaks(self.audio)

        pt = SMSPartialTracking()
        pt.max_partials = max_partials
        frames = pt.find_partials(frames)

        assert len(frames[0].partials) == max_partials
        assert frames[0].max_partials == max_partials

    def test_partial_tracking(self):
        pd = SMSPeakDetection()
        pd.max_peaks = max_peaks
        pd.hop_size = hop_size
        peaks = pd.find_peaks(self.audio)
        pt = SMSPartialTracking()
        pt.max_partials = max_partials
        frames = pt.find_partials(peaks)

        sms_frames = self.test_data['partial_tracking']
        sms_frames = sms_frames[0:len(sms_frames) - 3]

        assert len(sms_frames) == len(frames)

        for i in range(len(frames)):
            assert len(frames[i].partials) == len(sms_frames[i]['partials'])

            for p in range(len(frames[i].partials)):
                assert_almost_equals(frames[i].partials[p].amplitude,
                                     sms_frames[i]['partials'][p]['amplitude'],
                                     float_precision)
                assert_almost_equals(frames[i].partials[p].frequency,
                                     sms_frames[i]['partials'][p]['frequency'],
                                     float_precision)
                assert_almost_equals(frames[i].partials[p].phase,
                                     sms_frames[i]['partials'][p]['phase'],
                                     float_precision)


class TestSndObjPartialTracking(object):
    @classmethod
    def setup_class(cls):
        cls.audio = simpl.read_wav(audio_path)[0]
        cls.audio = cls.audio[0:num_samples]
        cls.test_data = _load_sndobj_test_data()

    def test_basic(self):
        pd = SndObjPeakDetection()
        pd.hop_size = hop_size
        pd.frame_size = hop_size
        pd.max_peaks = max_peaks
        frames = pd.find_peaks(self.audio)

        pt = SndObjPartialTracking()
        pt.max_partials = max_partials
        frames = pt.find_partials(frames)

        assert len(frames[0].partials) == max_partials
        assert frames[0].max_partials == max_partials

    def test_partial_tracking(self):
        pd = SndObjPeakDetection()
        pd.max_peaks = max_peaks
        pd.frame_size = hop_size
        pd.hop_size = hop_size
        peaks = pd.find_peaks(self.audio)
        pt = SndObjPartialTracking()
        pt.max_partials = max_partials
        frames = pt.find_partials(peaks)

        sndobj_frames = self.test_data['partial_tracking']

        assert len(sndobj_frames) == len(frames), \
            (len(sndobj_frames), len(frames))

        for i in range(len(frames)):
            print(sndobj_frames[i])
            assert len(frames[i].partials) == len(sndobj_frames[i])

            for p in range(len(frames[i].partials)):
                partial = frames[i].partials[p]
                sndobj_partial = sndobj_frames[i][p]
                assert_almost_equals(partial.amplitude,
                                     sndobj_partial['amplitude'],
                                     float_precision)
                assert_almost_equals(partial.frequency,
                                     sndobj_partial['frequency'],
                                     float_precision)
                assert_almost_equals(partial.phase,
                                     sndobj_partial['phase'],
                                     float_precision)
