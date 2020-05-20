module Page.SeamCarving.Msg exposing (Msg(..))

import Data.SeamCarving exposing (CarveImageResp)
import RemoteData as RD exposing (WebData)


type Msg
    = NoOp
    | CarveImage String
    | CarvedImage (WebData CarveImageResp)
    | HealthCheck
    | HealthChecked (WebData String)
