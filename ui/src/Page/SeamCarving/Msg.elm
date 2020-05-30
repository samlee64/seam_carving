module Page.SeamCarving.Msg exposing (GrowFormMsg(..), Msg(..))

import Bootstrap.Dropdown as Dropdown
import Data.SeamCarving exposing (CarveImageResp)
import Http
import RemoteData as RD exposing (WebData)


type Msg
    = NoOp
    | CarveImage String
    | CarvedImage (WebData CarveImageResp)
    | HealthCheck
    | HealthChecked (WebData String)
    | SelectImage String
    | UnselectImage
    | GrowFormMsg GrowFormMsg


type GrowFormMsg
    = ShowIntermediateSteps Bool
    | SetNumSteps Int
    | SetHeight String
    | SetWidth String
    | NumStepsDropdown Dropdown.State
