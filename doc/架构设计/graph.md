




``` json
pipeline_config.json
{
    "yolo_preproc_qualcommon_snpe": {
        "type": "YoloSNPEPreProcess",
        "params": {
            "input_width": 416,
            "input_height": 416,
            "input_channels": 3,
            "input_mean": 0.0,
            "input_std": 255.0,
            "preproc_type": "opencv"
        },
        "Inference": {
            "type": "SNPE",
            "params": {
                "runtime": "DSP",
                "runtime_list": ["DSP", "CPU"],
                "use_user_supplied_buffers": true,
                "platform_config": {
                    
                }
            }
        },
        "PostProcess": {
            "type": "YoloPostProcess",
            "params": {
                "input_width": 416,
                "input_height": 416,
                "input_channels": 3,
                "input_mean": 0.0,
                "input_std": 255.0,
            }
        }
    }
}
```