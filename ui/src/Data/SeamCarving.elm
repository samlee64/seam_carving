module Data.SeamCarving exposing (..)

import Json.Decode exposing (..)
import Json.Encode as E



--add the encoders/decoders here


type alias CarveImageParams =
    { imageName : String
    }


type alias CarveImageResp =
    { imageName : String }


type alias CarvedImage =
    {}


carveImageRespDecoder : Decoder CarveImageResp
carveImageRespDecoder =
    map CarveImageResp
        (field "imageName" string)
