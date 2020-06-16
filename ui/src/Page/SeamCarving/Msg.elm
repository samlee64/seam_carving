module Page.SeamCarving.Msg exposing (..)

import Bootstrap.Accordion as Accordion
import Bootstrap.Dropdown as Dropdown
import Bootstrap.Tab as Tab
import Data.Mouse exposing (..)
import Data.SeamCarving exposing (..)
import Http
import RemoteData as RD exposing (WebData)
import Time


type Msg
    = NoOp
    | SelectImage String
    | UnselectImage
    | GrowFormMsg GrowFormMsg
    | GrowImage
    | GrewImage (WebData GrowImageResp)
    | Tick Time.Posix
    | PolledExecutionStatus (WebData ExecutionStatusResp)
    | ContentAmplificationFormMsg ContentAmplificationFormMsg
    | AmplifyImage
    | AmplifiedImage (WebData ContentAmplificationResp)
    | TabMsg Tab.State
    | RemoveObjectFormMsg RemoveObjectFormMsg
    | RemovedObject RemoveObjectResp


type GrowFormMsg
    = ShowGrowIntermediateSteps Bool
    | SetNumSteps Int
    | SetHeight String
    | SetWidth String
    | NumStepsDropdown Dropdown.State


type ContentAmplificationFormMsg
    = ShowContentAmplificationIntermediateSteps Bool
    | SetFactor Float
    | FactorDropdown Dropdown.State


type RemoveObjectFormMsg
    = ShowRemoveObjectIntermediateSteps Bool
    | SetClickMode ClickMode
    | SetMarkMode MarkMode
    | MouseMove MouseMoveData
    | Click
    | SetLockRatio Bool
    | SetOnlyHorizontal Bool
    | SetOnlyVertical Bool
    | AccordionMsg Accordion.State


type ClickMode
    = Continious
    | Discreet


type MarkMode
    = Protect
    | Destroy
