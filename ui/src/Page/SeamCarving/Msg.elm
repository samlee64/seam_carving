module Page.SeamCarving.Msg exposing (Msg(..))

import Data.SeamCarving exposing (CarveImageResp)
import Http
import Image exposing (Image)
import RemoteData as RD exposing (WebData)


type Msg
    = NoOp
    | CarveImage String
    | CarvedImage (WebData CarveImageResp)
    | HealthCheck
    | HealthChecked (WebData String)
    | GetImage
    | GotImage (Result Http.Error (Maybe Image))
