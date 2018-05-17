<?php
class Maker {
	var $event;
	var $key;
	var $values;
	function __construct($event, $key)
	{
		$this->event = $event;
		$this->key = $key;
		$this->values = array();
	}
	function print_settings()
	{
		$settings = array('event' => $this->event, 'key' =>$this->key);
		echo json_encode($settings);
	}
	function set_values($values = array())
	{	
		$this->values = http_build_query($values);
	}
	function trigger()
	{
		//$url = 'https://maker.ifttt.com/trigger/'.$this->event.'/with/key/'.$this->key;
		$url = 'http://maker.ifttt.com/trigger/'.$this->event.'/with/key/'.$this->key;
		$curl = curl_init($url);
		$content = $this->values;
		curl_setopt($curl, CURLOPT_POST, 1);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_POSTFIELDS, $content);
		//curl_setopt($curl, CURLOPT_SSLVERSION, 3); 
		$response = curl_exec($curl);
		
		if(curl_error($curl))
{
    echo 'error:' . curl_error($curl);
}

		 echo 'response:';
		 echo $response ;
		 
		if($response){
			return true;
				echo 'true:';
		}else{
			return false;
			echo 'false:';
		}
		curl_close($curl);
	}
}
?>