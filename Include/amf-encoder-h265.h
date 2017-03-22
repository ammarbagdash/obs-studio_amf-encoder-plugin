/*
MIT License

Copyright (c) 2016-2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "plugin.h"
#include "amf-encoder.h"
#include "components/VideoEncoderHEVC.h"

namespace Plugin {
	namespace AMD {
		namespace H265 {
			enum class Tier : uint8_t {
				Main,
				High,
			};
			enum class GOPType : uint8_t {
				Fixed,		// Fixed Interval GOP
				Variable,	// Variable Interval GOP
			};
			enum class HeaderInsertionMode : uint8_t {
				None = 0,
				AlignedToGOP = 1,
				AlignedToIDR = 2,
			};
		}

		class EncoderH265 : public Encoder {
			public:
			EncoderH265(
				std::shared_ptr<API::IAPI> videoAPI, API::Adapter videoAdapter = API::Adapter::Adapter(),
				bool useOpenCLSubmission = false, bool useOpenCLConversion = false,
				ColorFormat colorFormat = ColorFormat::NV12, ColorSpace colorSpace = ColorSpace::BT709, bool fullRangeColor = false,
				bool useAsyncQueue = false, size_t asyncQueueSize = 0);
			virtual ~EncoderH265();

			// Initialization
			virtual std::vector<Usage> CapsUsage() override;
			virtual void SetUsage(Usage v) override;
			virtual Usage GetUsage() override;

			// Static
			virtual std::vector<QualityPreset> CapsQualityPreset() override;
			virtual void SetQualityPreset(QualityPreset v) override;
			virtual QualityPreset GetQualityPreset() override;

			virtual std::pair<std::pair<uint32_t, uint32_t>, std::pair<uint32_t, uint32_t>> CapsResolution() override;
			virtual void SetResolution(std::pair<uint32_t, uint32_t> v) override;
			virtual std::pair<uint32_t, uint32_t> GetResolution() override;

			virtual void SetAspectRatio(std::pair<uint32_t, uint32_t> v) override;
			virtual std::pair<uint32_t, uint32_t> GetAspectRatio() override;

			virtual void SetFrameRate(std::pair<uint32_t, uint32_t> v) override;
			virtual std::pair<uint32_t, uint32_t> GetFrameRate() override;

			virtual std::vector<Profile> CapsProfile() override;
			virtual void SetProfile(Profile v) override;
			virtual Profile GetProfile() override;

			virtual std::vector<ProfileLevel> CapsProfileLevel() override;
			virtual void SetProfileLevel(ProfileLevel v) override;
			virtual ProfileLevel GetProfileLevel() override;

			std::vector<H265::Tier> CapsTier();
			void SetTier(H265::Tier v);
			H265::Tier GetTier();

			virtual std::pair<uint64_t, uint64_t> CapsMaximumReferenceFrames() override;
			virtual void SetMaximumReferenceFrames(uint64_t v) override;
			virtual uint64_t GetMaximumReferenceFrames() override;

			virtual std::vector<CodingType> CapsCodingType() override;
			virtual void SetCodingType(CodingType v) override;
			virtual CodingType GetCodingType() override;

			virtual std::pair<uint32_t, uint32_t> CapsMaximumLongTermReferenceFrames() override;
			virtual void SetMaximumLongTermReferenceFrames(uint32_t v) override;
			virtual uint32_t GetMaximumLongTermReferenceFrames() override;

			/// Rate Control
			virtual std::vector<RateControlMethod> CapsRateControlMethod() override;
			virtual void SetRateControlMethod(RateControlMethod v) override;
			virtual RateControlMethod GetRateControlMethod() override;

			virtual std::vector<PrePassMode> CapsPrePassMode() override;
			virtual void SetPrePassMode(PrePassMode v) override;
			virtual PrePassMode GetPrePassMode() override;

			virtual void SetVarianceBasedAdaptiveQuantizationEnabled(bool v) override;
			virtual bool IsVarianceBasedAdaptiveQuantizationEnabled() override;

			/// VBV Buffer
			virtual std::pair<uint64_t, uint64_t> CapsVBVBufferSize() override;
			virtual void SetVBVBufferSize(uint64_t v) override;
			virtual uint64_t GetVBVBufferSize() override;

			virtual void SetVBVBufferInitialFullness(double v) override;
			virtual float GetInitialVBVBufferFullness() override;

			/// Picture Control
			std::vector<H265::GOPType> CapsGOPType();
			void SetGOPType(H265::GOPType v);
			H265::GOPType GetGOPType();

			void SetGOPSize(uint32_t v);
			uint32_t GetGOPSize();

			void SetGOPSizeMin(uint32_t v);
			uint32_t GetGOPSizeMin();

			void SetGOPSizeMax(uint32_t v);
			uint32_t GetGOPSizeMax();

			virtual void SetGOPAlignmentEnabled(bool v) override;
			virtual bool IsGOPAlignmentEnabled() override;

			virtual void SetIDRPeriod(uint32_t v) override; // Distance in GOPs
			virtual uint32_t GetIDRPeriod() override;

			void SetHeaderInsertionMode(H265::HeaderInsertionMode v);
			H265::HeaderInsertionMode GetHeaderInsertionMode();

			virtual void SetDeblockingFilterEnabled(bool v) override;
			virtual bool IsDeblockingFilterEnabled() override;

			/// Motion Estimation
			virtual void SetMotionEstimationQuarterPixelEnabled(bool v) override;
			virtual bool IsMotionEstimationQuarterPixelEnabled() override;

			virtual void SetMotionEstimationHalfPixelEnabled(bool v) override;
			virtual bool IsMotionEstimationHalfPixelEnabled() override;

			// Dynamic
			virtual void SetFrameSkippingEnabled(bool v) override;
			virtual bool IsFrameSkippingEnabled() override;

			virtual void SetEnforceHRDEnabled(bool v) override;
			virtual bool IsEnforceHRDEnabled() override;

			virtual void SetFillerDataEnabled(bool v) override;
			virtual bool IsFillerDataEnabled() override;

			void SetIFrameQPMinimum(uint8_t v);
			uint8_t GetIFrameQPMinimum();

			void SetIFrameQPMaximum(uint8_t v);
			uint8_t GetIFrameQPMaximum();

			void SetPFrameQPMinimum(uint8_t v);
			uint8_t GetPFrameQPMinimum();

			void SetPFrameQPMaximum(uint8_t v);
			uint8_t GetPFrameQPMaximum();

			virtual std::pair<uint64_t, uint64_t> CapsTargetBitrate() override;
			virtual void SetTargetBitrate(uint64_t v) override;
			virtual uint64_t GetTargetBitrate() override;

			virtual std::pair<uint64_t, uint64_t> CapsPeakBitrate() override;
			virtual void SetPeakBitrate(uint64_t v) override;
			virtual uint64_t GetPeakBitrate() override;

			virtual void SetIFrameQP(uint8_t v) override;
			virtual uint8_t GetIFrameQP() override;

			virtual void SetPFrameQP(uint8_t v) override;
			virtual uint8_t GetPFrameQP() override;

			virtual void SetMaximumAccessUnitSize(uint32_t v) override;
			virtual uint32_t GetMaximumAccessUnitSize() override;

			/// Intra-Refresh
			void SetIntraRefreshMode(uint32_t v);	// Description is identical to IntraRefreshNumMBsPerSlot?
			uint32_t GetIntraRefreshMode();			// Does not seem to be an actual property yet.
			
			void SetIntraRefreshFrameNum(uint32_t v);
			uint32_t GetIntraRefreshFrameNum();

			/// Slicing
			virtual std::pair<uint32_t, uint32_t> CapsSlicesPerFrame() override;
			virtual void SetSlicesPerFrame(uint32_t v) override;
			virtual uint32_t GetSlicesPerFrame() override;

			virtual void SetSliceControlMode(SliceControlMode v) override;
			virtual SliceControlMode GetSliceControlMode() override;

			virtual std::pair<uint32_t, uint32_t> CapsSliceControlSize() override;
			virtual void SetSliceControlSize(uint32_t v) override;
			virtual uint32_t GetSliceControlSize() override;
			
			// Experimental
			void SetQPCBOffset(uint8_t v);
			uint8_t GetQPCBOffset();

			void SetQPCROffset(uint8_t v);
			uint8_t GetQPCROffset();

			std::pair<uint32_t, uint32_t> CapsInputQueueSize();
			void SetInputQueueSize(uint32_t v);
			uint32_t GetInputQueueSize();

			virtual void SetLowLatencyInternal(bool v) override;
			virtual bool GetLowLatencyInternal() override;

			virtual void SetCommonLowLatencyInternal(bool v) override;
			virtual bool GetCommonLowLatencyInternal() override;

			// Internal
			virtual void LogProperties() override;
			protected:
			virtual void PacketPriorityAndKeyframe(amf::AMFDataPtr& d, struct encoder_packet* p) override;
			virtual AMF_RESULT GetExtraDataInternal(amf::AMFVariant* p) override;




			//Remaining Properties
			// PerformanceCounter (Interface, but which one?)
			// HevcMaxNumOfTemporalLayers/HevcNumOfTemporalLayers/HevcTemporalLayerSelect - Only supports QP_I/P?
			// BPicturesPattern (replaced by merge mode?)
			// HevcMaxMBPerSec (PCI-E bandwidth, min/max)
		};
	}
}